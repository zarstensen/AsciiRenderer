#include "arpch.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Core/Application.h"
#include "Asciir/Core/Terminal.h"
#include "Asciir/Input/EventListener.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	Coord Input::s_mouse_pos, Input::s_mouse_diff, Input::s_last_terminal_pos;
	TermVert Input::s_last_size, Input::s_cur_pos, Input::s_cur_diff;
	TerminalRenderer::TRUpdateInfo Input::s_info;
	Ref<EventListener> Input::s_event_listener;

	// structure storing polled event listener data, as well as storing a toggled attribute,
	// that allows the input class to detect when the key is toggled on / off, relative to the event listener polls.
	struct KeyToggledData
	{
		bool is_down = false;
		bool is_toggled = false;

		KeyToggledData& operator=(EventListener::KeyInputData data)
		{
			is_down = data.is_down;

			return *this;
		}
	};

	// same as InputKeyData, but for mouse data
	struct MouseToggledData
	{
		bool is_down = false;
		bool is_toggled = false;

		MouseToggledData& operator=(EventListener::MouseInputData data)
		{
			is_down = data.is_down;

			return *this;
		}
	};

	// array of all the keys that could possibly be polled by the event listener.
	static std::array<KeyToggledData, KEY_CODE_COUNT>		key_toggled_state = { KeyToggledData() };
	static std::array<MouseToggledData, MOUSE_CODE_COUNT>	mouse_toggled_state = { MouseToggledData() };

	void Input::setEventListener(Ref<EventListener> listener)
	{
		s_event_listener = listener;
	}

	bool Input::isKeyDown(Key keycode)
	{
		return s_event_listener->getKeyFromCode(keycode).is_down && isFocused();
	}

	bool Input::isKeyUp(Key keycode)
	{
		return !s_event_listener->getKeyFromCode(keycode).is_down && isFocused();
	}

	bool Input::isKeyPressed(Key keycode)
	{
		EventListener::KeyInputData key_data = s_event_listener->getKeyFromCode(keycode);
		return castMilli(getTime() - key_data.time_since_down) > AR_KEY_PRESSED_TIMEOUT && key_data.is_down && isFocused();
	}

	bool Input::isKeyToggled(Key keycode)
	{
		EventListener::KeyInputData key_data = s_event_listener->getKeyFromCode(keycode);
		return key_toggled_state[(size_t)keycode - 1].is_toggled && key_data.is_down && isFocused();
	}

	bool Input::isKeyUntoggled(Key keycode)
	{
		EventListener::KeyInputData key_data = s_event_listener->getKeyFromCode(keycode);
		return key_toggled_state[(size_t)keycode - 1].is_toggled && !key_data.is_down && isFocused();
	}

	bool Input::isMouseDown(MouseKey keycode)
	{
		return s_event_listener->getMouseKeyFromCode(keycode).is_down && isFocused();
	}

	bool Input::isMouseUp(MouseKey keycode)
	{
		return !s_event_listener->getMouseKeyFromCode(keycode).is_down && isFocused();
	}

	bool Input::isMouseToggled(MouseKey keycode)
	{
		EventListener::MouseInputData mouse_data = s_event_listener->getMouseKeyFromCode(keycode);
		return mouse_toggled_state[(size_t)keycode - 1].is_toggled && mouse_data.is_down && isFocused();
	}

	bool Input::isMouseUntoggled(MouseKey keycode)
	{
		EventListener::MouseInputData mouse_data = s_event_listener->getMouseKeyFromCode(keycode);
		return mouse_toggled_state[(size_t)keycode - 1].is_toggled && !mouse_data.is_down && isFocused();
	}

	bool Input::isMouseMoved()
	{
		return (s_mouse_diff != Coord(0, 0)) && isFocused();
	}

	bool Input::isTerminalMoved()
	{
		return ARApp::getApplication()->getTermRenderer().pos() != s_last_terminal_pos;
	}

	bool Input::isTerminalResized()
	{
		return ARApp::getApplication()->getTermRenderer().termSize() != s_last_size;
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
		if (isMouseDown(keycode))
		{
			return MousePressedEvent(keycode, s_event_listener->getMousePos(), s_event_listener->getCursorPos(), s_event_listener->getMouseKeyFromCode(keycode).is_double_click);
		}
		else if (isMouseUp(keycode))
		{
			return MouseReleasedEvent(keycode, s_event_listener->getMousePos(), s_event_listener->getCursorPos());
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
			ARApp::getApplication()->getTermRenderer().pos(),
			ARApp::getApplication()->getTermRenderer().pos() - s_last_terminal_pos, false);
	}

	TerminalResizedEvent Input::getTerminalResizedEvent()
	{
		return TerminalResizedEvent(
			ARApp::getApplication()->getTermRenderer().termSize(),
			ARApp::getApplication()->getTermRenderer().termSize() - s_last_size, false);
	}

	void Input::pollState(TerminalRenderer::TRUpdateInfo info)
	{
		s_mouse_diff = s_event_listener->getMousePos() - s_mouse_pos;
		s_cur_diff = s_event_listener->getCursorPos() - s_cur_pos;

		s_mouse_pos = s_event_listener->getMousePos();
		s_cur_pos = s_event_listener->getCursorPos();

		s_info = info;
		s_last_terminal_pos = ARApp::getApplication()->getTermRenderer().pos();
		s_last_size = ARApp::getApplication()->getTermRenderer().termSize();

		const auto& keybd_poll = s_event_listener->getKeybdPoll();
		const auto& mouse_poll = s_event_listener->getMousePoll();

		for (size_t i = 0; i < KEY_CODE_COUNT; i++)
		{
			const EventListener::KeyInputData& poll_data = keybd_poll[i];
			KeyToggledData& input_key_data = key_toggled_state[i];

			if (poll_data.is_down != input_key_data.is_down)
				input_key_data.is_toggled = true;
			else if (poll_data.is_down == input_key_data.is_down)
				input_key_data.is_toggled = false;

			input_key_data = poll_data;
		}

		for (size_t i = 0; i < MOUSE_CODE_COUNT; i++)
		{
			EventListener::MouseInputData poll_data = mouse_poll[i];
			MouseToggledData& input_mouse_data = mouse_toggled_state[i];

			if (poll_data.is_down != input_mouse_data.is_down)
				input_mouse_data.is_toggled = true;
			else if (poll_data.is_down == input_mouse_data.is_down)
				input_mouse_data.is_toggled = false;

			input_mouse_data = poll_data;
		}
	}

	Coord Input::getMousePos()
	{
		return s_event_listener->getCurrentMousePos();
	}
}