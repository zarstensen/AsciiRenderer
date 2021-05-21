#include "arpch.h"
#include "WinInput.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Core/Terminal.h"
#include "Asciir/Logging/Log.h"
#include "WindowsARAttributes.h"
#include "KeyCodeMap.h"

namespace Asciir
{
	Coord Input::s_last_mouse_pos, Input::s_last_terminal_pos;
	TermVert Input::s_last_size;
	TRUpdateInfo Input::s_info;

	bool Input::isKeyDown(Key keycode)
	{
		return WIN_KEY_MAP[keyCodeToWin(keycode)].is_down && isFocused();
	}

	bool Input::isKeyUp(Key keycode)
	{
		return !WIN_KEY_MAP[keyCodeToWin(keycode)].is_down && isFocused();
	}

	bool Input::isKeyPressed(Key keycode)
	{
		KeyInputData& key_data = WIN_KEY_MAP[keyCodeToWin(keycode)];
		return castMilli(getTime() - key_data.time_since_down) > AR_KEY_PRESSED_TIMEOUT && key_data.is_down && isFocused();
	}

	bool Input::isKeyToggled(Key keycode)
	{
		KeyInputData& key_data = WIN_KEY_MAP[keyCodeToWin(keycode)];
		return key_data.is_toggled && key_data.is_down && isFocused();
	}

	bool Input::isKeyUntoggled(Key keycode)
	{
		KeyInputData& key_data = WIN_KEY_MAP[keyCodeToWin(keycode)];
		return key_data.is_toggled && !key_data.is_down && isFocused();
	}

	bool Input::isMouseDown(MouseKey keycode)
	{
		return WIN_KEY_MAP[mouseCodeToWin(keycode)].is_down && isFocused();
	}

	bool Input::isMouseUp(MouseKey keycode)
	{
		return !WIN_KEY_MAP[mouseCodeToWin(keycode)].is_down && isFocused();
	}

	bool Input::isMouseToggled(MouseKey keycode)
	{
		KeyInputData mouse_data = WIN_KEY_MAP[mouseCodeToWin(keycode)];
		return mouse_data.is_toggled && mouse_data.is_down && isFocused();
	}

	bool Input::isMouseUntoggled(MouseKey keycode)
	{
		KeyInputData mouse_data = WIN_KEY_MAP[mouseCodeToWin(keycode)];
		return mouse_data.is_toggled && !mouse_data.is_down && isFocused();
	}

	bool Input::isMouseMoved()
	{
		return getMousePos() != s_last_mouse_pos && isFocused();
	}

	bool Input::isTerminalMoved()
	{
		return AREngine::getEngine()->getTerminal()->getPos() != s_last_terminal_pos;
	}

	bool Input::isTerminalResized()
	{
		return AREngine::getEngine()->getTerminal()->getSize() != s_last_size;
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

		WinARAttr& s_attr_handler = dynamic_cast<WinARAttr&>(AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler());

		Coord mouse_pos = getMousePos();
		TermVert cur_pos = (mouse_pos - AREngine::getEngine()->getTerminal()->getPos()).cwiseQuotient((Coord)s_attr_handler.fontSize()) - Coord(1, 2);

		if (isMouseDown(keycode))
		{
			return MousePressedEvent(keycode, mouse_pos, cur_pos);
		}
		else if (isMouseUp(keycode))
		{
			return MouseReleasedEvent(keycode, mouse_pos, cur_pos);
		}
		AR_ASSERT_MSG(false, "Key was neither pressed or released (Terminal not in focus?)");
		return {};
	}

	MouseMovedEvent Input::getMouseMovedEvent()
	{
		WinARAttr& s_attr_handler = dynamic_cast<WinARAttr&>(AREngine::getEngine()->getTerminal()->getRenderer()->getAttrHandler());
		Coord mouse_pos = getMousePos();
		TermVert cur_pos = (mouse_pos - AREngine::getEngine()->getTerminal()->getPos()).cwiseQuotient((Coord) s_attr_handler.fontSize()) - Coord(1, 2);
		TermVert last_cur_pos = (s_last_mouse_pos - s_last_terminal_pos).cwiseQuotient((Coord) s_attr_handler.fontSize()) - Coord(1, 2);

		return MouseMovedEvent(mouse_pos, mouse_pos - s_last_mouse_pos, cur_pos, cur_pos - last_cur_pos);
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
		SHORT keyboard_state[ks_len];

		// fill keyboard_state with data from GetAsyncKeyState
		for (int i = 0; i < ks_len; i++)
			keyboard_state[i] = GetAsyncKeyState(i);

		for (int i = 0; i < ks_len; i++)
		{

 			KeyInputData& key_data = WIN_KEY_MAP[i];
			SHORT key = keyboard_state[i];

			// only set toggled to true if it was not pressed before
			if (CHECK_BIT(key, 0) && !key_data.is_down)
			{
				key_data.is_toggled = true;
				key_data.time_since_down = getTime();
			}
			else
			{
				key_data.is_toggled = false;
			}


			if (CHECK_BIT(key, 0))
			{
				key_data.is_down = true;
			}
			else if (!CHECK_BIT(key, 0) && key_data.is_down)
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
		POINT pos;
		AR_WIN_VERIFY(GetCursorPos(&pos));
		return pos;
	}
}
