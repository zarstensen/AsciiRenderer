#include "arpch.h"
#include "WinInput.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Core/Terminal.h"
#include "WinEventListener.h"
#include "Asciir/Logging/Log.h"
#include "WindowsARAttributes.h"
#include "KeyCodeMap.h"

namespace Asciir
{
	Coord Input::s_mouse_pos, Input::s_mouse_diff, Input::s_last_terminal_pos;
	TermVert Input::s_last_size, Input::s_cur_pos, Input::s_cur_diff;
	TerminalRenderer::TRUpdateInfo Input::s_info;
	std::shared_ptr<EventListener> Input::s_event_listener;

	// global windows specific variables

	struct WinInKeyData
	{
		bool is_down = false;
		bool is_toggled = false;

		WinInKeyData& operator=(EventListener::KeyInputData data)
		{
			is_down = data.is_down;
			
			return *this;
		}

	};

	struct WinInMouseData
	{
		bool is_down = false;
		bool is_toggled = false;

		WinInMouseData& operator=(EventListener::MouseInputData data)
		{
			is_down = data.is_down;

			return *this;
		}
	};

	static std::shared_ptr<WinEventListener> win_listener;
	static std::array<WinInKeyData, KEY_CODE_COUNT>		key_toggled_state = { WinInKeyData() };
	static std::array<WinInMouseData, MOUSE_CODE_COUNT>	mouse_toggled_state = { WinInMouseData() };

	void Input::setEventListener(std::shared_ptr<EventListener> listener)
	{
		s_event_listener = listener;
		win_listener = dynamic_pointer_cast<WinEventListener>(s_event_listener);
	}

	bool Input::isKeyDown(Key keycode)
	{
		return win_listener->getKeybdFromKeyCode(keycode).is_down && isFocused();
	}

	bool Input::isKeyUp(Key keycode)
	{
		return !win_listener->getKeybdFromKeyCode(keycode).is_down && isFocused();
	}

	bool Input::isKeyPressed(Key keycode)
	{
		EventListener::KeyInputData key_data = win_listener->getKeybdFromKeyCode(keycode);
		return castMilli(getTime() - key_data.time_since_down) > AR_KEY_PRESSED_TIMEOUT && key_data.is_down && isFocused();
	}

	bool Input::isKeyToggled(Key keycode)
	{
		EventListener::KeyInputData key_data = win_listener->getKeybdFromKeyCode(keycode);
		return key_toggled_state[(size_t) keycode - 1].is_toggled && key_data.is_down && isFocused();
		return false;
	}

	bool Input::isKeyUntoggled(Key keycode)
	{
		EventListener::KeyInputData key_data = win_listener->getKeybdFromKeyCode(keycode);
		return key_toggled_state[(size_t)keycode - 1].is_toggled && !key_data.is_down && isFocused();
		return false;
	}

	bool Input::isMouseDown(MouseKey keycode)
	{
		return win_listener->getMouseFromKeyCode(keycode).is_down && isFocused();
	}

	bool Input::isMouseUp(MouseKey keycode)
	{
		return !win_listener->getMouseFromKeyCode(keycode).is_down && isFocused();
	}

	bool Input::isMouseToggled(MouseKey keycode)
	{
		EventListener::MouseInputData mouse_data = win_listener->getMouseFromKeyCode(keycode);
		return mouse_toggled_state[(size_t)keycode - 1].is_toggled && mouse_data.is_down && isFocused();
		return false;
	}

	bool Input::isMouseUntoggled(MouseKey keycode)
	{
		EventListener::MouseInputData mouse_data = win_listener->getMouseFromKeyCode(keycode);
		return mouse_toggled_state[(size_t)keycode - 1].is_toggled && !mouse_data.is_down && isFocused();
		return false;
	}

	bool Input::isMouseMoved()
	{
		return (s_mouse_diff != Coord(0, 0)) && isFocused();
	}

	bool Input::isTerminalMoved()
	{
		return AREngine::getEngine()->getTerminal().getPos() != s_last_terminal_pos;
	}

	bool Input::isTerminalResized()
	{
		return AREngine::getEngine()->getTerminal().getSize() != s_last_size;
	}

	bool Input::isFocused()
	{
		return GetForegroundWindow() == GetConsoleWindow();
	}

	std::variant<std::monostate, KeyPressedEvent, KeyReleasedEvent> Input::getKeyEvent(Key keycode)
	{
		if (isKeyDown(keycode))
			return KeyPressedEvent(keycode, isKeyPressed(keycode));
		else if (isKeyUp(keycode))
			return KeyReleasedEvent(keycode);
		AR_ASSERT_MSG(false, "Key was neither pressed or released (Terminal not in focus?)");
		return {};
	}

	

	std::variant<std::monostate, MousePressedEvent, MouseReleasedEvent> Input::getMouseKeyEvent(MouseKey keycode)
	{

		// calculate mouse position on terminal by subtracting the terminal position and dividing by the font size
		// also offset by 2 in the y and 1 in the x to get the correct cursor pos

		WinARAttr& s_attr_handler = dynamic_cast<WinARAttr&>(AREngine::getEngine()->getTerminal().getRenderer()->getAttrHandler());

		if (isMouseDown(keycode))
		{
			return MousePressedEvent(keycode, win_listener->getMousePos(), win_listener->getCursorPos(), win_listener->getMouseFromKeyCode(keycode).is_double_click);
		}
		else if (isMouseUp(keycode))
		{
			return MouseReleasedEvent(keycode, win_listener->getMousePos(), win_listener->getCursorPos());
		}

		AR_ASSERT_MSG(false, "Key was neither pressed or released (Terminal not in focus?)");
		return {};
	}

	MouseMovedEvent Input::getMouseMovedEvent()
	{
		return MouseMovedEvent(s_mouse_pos, s_mouse_diff, s_cur_pos, s_cur_diff);
	}

	TerminalMovedEvent Input::getTerminalMovedEvent()
	{
		return TerminalMovedEvent(
			AREngine::getEngine()->getTerminal().getPos(),
			AREngine::getEngine()->getTerminal().getPos() - s_last_terminal_pos, false);
	}

	TerminalResizedEvent Input::getTerminalResizedEvent()
	{
		return TerminalResizedEvent(
			AREngine::getEngine()->getTerminal().getSize(),
			AREngine::getEngine()->getTerminal().getSize() - s_last_size, false);
	}

	void Input::pollState(TerminalRenderer::TRUpdateInfo info)
	{
		s_mouse_diff = win_listener->getMousePos() - s_mouse_pos;
		s_cur_diff = win_listener->getCursorPos() - s_cur_pos;

		s_mouse_pos = win_listener->getMousePos();
		s_cur_pos = win_listener->getCursorPos();

		s_info = info;
		s_last_terminal_pos = AREngine::getEngine()->getTerminal().getPos();
		s_last_size = AREngine::getEngine()->getTerminal().getSize();

		const auto& keybd_poll = win_listener->getKeybdPoll();
		const auto& mouse_poll = win_listener->getMousePoll();

		for (size_t i = 0; i < KEY_CODE_COUNT; i++)
		{
			EventListener::KeyInputData poll_data = keybd_poll[i];
			WinInKeyData& input_key_data = key_toggled_state[i];

			if (poll_data.is_down != input_key_data.is_down)
				input_key_data.is_toggled = true;
			else if (poll_data.is_down == input_key_data.is_down)
				input_key_data.is_toggled = false;

			input_key_data = poll_data;
		}
		
		for (size_t i = 0; i < MOUSE_CODE_COUNT; i++)
		{
			EventListener::MouseInputData poll_data = mouse_poll[i];
			WinInMouseData& input_mouse_data = mouse_toggled_state[i];
			
			if (poll_data.is_down != input_mouse_data.is_down)
				input_mouse_data.is_toggled = true;
			else if (poll_data.is_down == input_mouse_data.is_down)
				input_mouse_data.is_toggled = false;

			input_mouse_data = poll_data;
		}
	}

	Coord Input::getMousePos()
	{
		POINT pos;
		AR_WIN_VERIFY(GetCursorPos(&pos));
		return pos;
	}
}
