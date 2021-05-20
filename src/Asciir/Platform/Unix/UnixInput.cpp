#include "arpch.h"
#include "UnixInput.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Core/Terminal.h"
#include "Asciir/Logging/Log.h"
#include "UnixARAttributes.h"
#include "KeyCodeMap.h"

namespace Asciir
{
	Coord Input::s_last_mouse_pos, Input::s_last_terminal_pos;
	TermVert Input::s_last_size;
	TRUpdateInfo Input::s_info;

	// key and mouse functions currently use the same code
	// but this will be changed when custom keycodes are added

	bool Input::isKeyDown(Key keycode)
	{
		return UNIX_KEY_MAP[keyCodeToUnix(keycode)].is_down;
	}

	bool Input::isKeyUp(Key keycode)
	{
		return !UNIX_KEY_MAP[keyCodeToUnix(keycode)].is_down;
	}

	bool Input::isKeyPressed(Key keycode)
	{
		KeyInputData& key_data = UNIX_KEY_MAP[keyCodeToUnix(keycode)];
		return castMilli(getTime() - key_data.time_since_down) > AR_KEY_PRESSED_TIMEOUT && key_data.is_down;
	}

	bool Input::isKeyToggled(Key keycode)
	{
		KeyInputData& key_data = UNIX_KEY_MAP[keyCodeToUnix(keycode)];
		return key_data.is_toggled && key_data.is_down;
	}

	bool Input::isKeyUntoggled(Key keycode)
	{
		KeyInputData& key_data = UNIX_KEY_MAP[keyCodeToUnix(keycode)];
		return key_data.is_toggled && !key_data.is_down;
  	}

	bool Input::isMouseDown(MouseKey keycode)
	{
		return UNIX_KEY_MAP[mouseCodeToUnix(keycode)].is_down;
	}

	bool Input::isMouseUp(MouseKey keycode)
	{
		return !UNIX_KEY_MAP[mouseCodeToUnix(keycode)].is_down;
	}

	bool Input::isMouseToggled(MouseKey keycode)
	{
		KeyInputData mouse_data = UNIX_KEY_MAP[mouseCodeToUnix(keycode)];
		return mouse_data.is_toggled && mouse_data.is_down;
	}

	bool Input::isMouseUntoggled(MouseKey keycode)
	{
		KeyInputData mouse_data = UNIX_KEY_MAP[mouseCodeToUnix(keycode)];
		return mouse_data.is_toggled && !mouse_data.is_down;
	}

	bool Input::isMouseMoved()
	{
		return getMousePos() != s_last_mouse_pos;
	}

	bool Input::isTerminalMoved()
	{
		return AREngine::getEngine()->getTerminal()->getPos() != s_last_terminal_pos;
	}

	bool Input::isTerminalResized()
	{
		return AREngine::getEngine()->getTerminal()->getSize() != s_last_size;
	}

	std::variant<std::monostate, KeyPressedEvent, KeyReleasedEvent> Input::getKeyEvent(Key keycode)
	{
		if (isKeyDown(keycode))
			return KeyPressedEvent(keycode, isKeyPressed(keycode));
		else if (isKeyUp(keycode))
			return KeyReleasedEvent(keycode);
		AR_ASSERT_MSG(false, "Key was neither pressed or released");
		return {};
	}

	

	std::variant<std::monostate, MousePressedEvent, MouseReleasedEvent> Input::getMouseKeyEvent(MouseKey keycode)
	{
		if (isMouseDown(keycode))
			return MousePressedEvent(keycode);
		else if (isMouseUp(keycode))
			return MouseReleasedEvent(keycode);
		AR_ASSERT_MSG(false, "Key was neither pressed or released");
		return {};
	}

	MouseMovedEvent Input::getMouseMovedEvent()
	{
		return MouseMovedEvent(getMousePos(), (Coord)(getMousePos() - s_last_mouse_pos).eval());
	}

	TerminalMovedEvent Input::getTerminalMovedEvent()
	{
		return TerminalMovedEvent(
			AREngine::getEngine()->getTerminal()->getPos(),
			AREngine::getEngine()->getTerminal()->getPos() - s_last_terminal_pos, false);
	}

	TerminalResizedEvent Input::getTerminalResizedEvent()
	{
		return TerminalResizedEvent(
			AREngine::getEngine()->getTerminal()->getSize(),
			AREngine::getEngine()->getTerminal()->getSize() - s_last_size, false);
	}

	void Input::update(TRUpdateInfo info)
	{
		s_info = info;
		s_last_mouse_pos = getMousePos();
		s_last_terminal_pos = AREngine::getEngine()->getTerminal()->getPos();
		s_last_size = AREngine::getEngine()->getTerminal()->getSize();

		constexpr int ks_len = 256;
		std::array<XEvent, ks_len> keyboard_state;

		// fill keyboard_state with previous key states

		for(int i = 0; i < ks_len; i++)
		{
			keyboard_state[i] = XEvent{LASTEvent};
		}

		// update keyboard_state with events from X11
		UnixARAttribute& h_attr = AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler();
		Display* display = h_attr.m_display;
		Window window = h_attr.m_window;
		Window focus_window = h_attr.m_focus_win;



		while(XPending(display))
		{
			XEvent event;
			XNextEvent(display, &event);

			switch(event.type)
			{
				case FocusOut:
				{
					int revert;

					if (focus_window != window)
                		XSelectInput(display, focus_window, NoEventMask);

           			XGetInputFocus(display, &focus_window, &revert);

            		if (focus_window == PointerRoot)
                		focus_window = window;

            		XSelectInput(display, focus_window, KeyPressMask | KeyReleaseMask | FocusChangeMask);
				}

				default:
					keyboard_state[event.xkey.keycode] = event;

			}

			Window r_win, c_win;
			int r_x, r_y, w_x, w_y;
			unsigned int return_mask;

        	XQueryPointer(display, window, &r_win, &c_win, &r_x, &r_y, &w_x, &w_y, &return_mask);
		}

		for (int i = 0; i < ks_len; i++)
		{

 			KeyInputData& key_data = UNIX_KEY_MAP[i];
			XEvent key_event = keyboard_state[i];

			// only set toggled to true if it was not pressed before
			if (key_event.type == KeyPress && !key_data.is_down)
			{
				key_data.is_toggled = true;
				key_data.time_since_down = getTime();
			}
			else
			{
				key_data.is_toggled = false;
			}


			if (key_event.type == KeyPress)
			{
				key_data.is_down = true;
			}
			else if (key_event.type == KeyRelease && key_data.is_down)
			{
				key_data.is_down = false;
				key_data.is_toggled = true;
			}
			else
				key_data.is_down = false;
		}

	}

	Coord Input::getMousePos()
	{
		Display* x_display = AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler().m_display;
		Window x_win = AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler().m_window;
		Window r_win, c_win;
		int r_x, r_y, w_x, w_y;
		unsigned int mask_return;

		AR_VERIFY(XQueryPointer(x_display, x_win, &r_win, &c_win, &r_x, &r_y, &w_x, &w_y, &mask_return));

		return { r_x, r_y };
	}
}
