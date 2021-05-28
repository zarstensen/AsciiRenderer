#include "pch/arpch.h"
#include "WinEventListener.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Core/Engine.h"
#include "KeyCodeMap.h"


namespace Asciir
{
	WinEventListener::WinEventListener()
	{
		m_hConsole_in = GetStdHandle(STD_INPUT_HANDLE);
		m_hConsole_out = GetStdHandle(STD_OUTPUT_HANDLE);
		AR_WIN_VERIFY(m_hConsole_in);
		AR_WIN_VERIFY(m_hConsole_out);

		AR_WIN_VERIFY(GetConsoleMode(m_hConsole_in, &m_hcin_fallback));
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback & ~ENABLE_QUICK_EDIT_MODE | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT));

		
	}

	WinEventListener::~WinEventListener()
	{
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback));
	}

	void WinEventListener::start(EventCallbackFp callback)
	{
		m_attr = dynamic_cast<WinARAttr*>(&AREngine::getEngine()->getTerminal().getRenderer()->getAttrHandler());

		m_last_term_pos = m_attr->terminalPos();
		m_last_term_size = m_attr->terminalSize();

		m_callback = callback;
		m_is_listening = true;
		m_input_thrd = std::thread(&WinEventListener::listenForInputs, this);
	}

	void WinEventListener::stop()
	{
		m_is_listening = false;
		m_input_thrd.join();
	}

	EventListener::KeyInputData WinEventListener::getKeybdFromWinCode(WORD code) const
	{
		return keybd_state[(size_t) WinToKeyCodeMap.at(code) - 1];
	}

	EventListener::KeyInputData WinEventListener::getKeybdFromKeyCode(Key code) const
	{
		return keybd_state[(size_t)code - 1];
	}

	EventListener::MouseInputData WinEventListener::getMouseFromWinCode(WORD code) const
	{
		return mouse_state[(size_t)WinToKeyCodeMap.at(code) - 1];
	}

	EventListener::MouseInputData WinEventListener::getMouseFromKeyCode(MouseKey code) const
	{
		return mouse_state[(size_t)code - 1];
	}

	void WinEventListener::listenForInputs()
	{
		
		while (m_is_listening)
		{
			DWORD num_events;
			AR_WIN_VERIFY(GetNumberOfConsoleInputEvents(m_hConsole_in, &num_events));

			if (num_events > 0)
			{
				DWORD num_read;
				AR_WIN_VERIFY(ReadConsoleInput(m_hConsole_in, m_event_buffer, BUFF_LEN, &num_read));

				for (size_t i = 0; i < num_read; i++)
				{
					INPUT_RECORD event_r = m_event_buffer[i];
					switch (event_r.EventType)
					{
					case KEY_EVENT:
						sendKeybdEvent(event_r.Event.KeyEvent);
						break;
					case MOUSE_EVENT:
						sendMouseEvent(event_r.Event.MouseEvent);
						break;
					case FOCUS_EVENT:
						sendFocusEvent(event_r.Event.FocusEvent);
						break;
					case WINDOW_BUFFER_SIZE_EVENT:
						break;
					case MENU_EVENT:
						break;
					default:
						AR_CORE_WARN("Unknown windows console event type: ", m_event_buffer[i].EventType);
						break;
					}
				}
			}

			sendTermEvents();
		}
	}

	void WinEventListener::sendKeybdEvent(KEY_EVENT_RECORD event)
	{
		if (event.bKeyDown)
		{
			KeyInputData& data = keybd_state[(size_t) WinToKeyCodeMap.at(event.wVirtualKeyCode) - 1];
			KeyPressedEvent e;
			
			if (!data.is_down)
			{
				data.is_repeat = true;
				data.is_down = true;
				data.time_since_down = getTime();

				e = KeyPressedEvent(WinToKeyCodeMap.at(event.wVirtualKeyCode), false);
			}
			else if (data.is_down)
			{
				data.is_repeat = false;

				e = KeyPressedEvent(WinToKeyCodeMap.at(event.wVirtualKeyCode), true);
			}

			m_callback(e);
		}
		else
		{
			KeyInputData& data = keybd_state[(size_t) WinToKeyCodeMap.at(event.wVirtualKeyCode) - 1];
			KeyReleasedEvent e(WinToKeyCodeMap.at(event.wVirtualKeyCode));

			if (data.is_down)
			{
				data.is_repeat = true;
				data.is_down = false;
			}
			else if (!data.is_down)
			{
				data.is_repeat = false;
			}

			m_callback(e);
		}
	}

	// mouse pos on the screen is found when the event is recieved so it might be off compared to when the event was sent
	void WinEventListener::sendMouseEvent(MOUSE_EVENT_RECORD event)
	{
		
		for (int button = 0; button < 5; button++)
		{
			WORD button_word = BIT_SHL(button);
			
			if (event.dwEventFlags & DOUBLE_CLICK)
			{

				if (CHECK_BIT(event.dwButtonState, button))
				{
					MouseInputData& data = mouse_state[button];
					data.is_double_click = true;

					TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };
					MousePressedEvent e(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos, true);
					m_callback(e);
				}
				else
					mouse_state[button].is_double_click = false;
			}
			else if (event.dwEventFlags == 0)
			{
				if (CHECK_BIT(event.dwButtonState, button))
				{
					MouseInputData& data = mouse_state[button];

					

					if (!data.is_down)
					{
						data.is_down = true;
						data.time_since_down = getTime();

						TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };
						MousePressedEvent e(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos, false);
						
						m_callback(e);
					}
				}
				else
				{
					MouseInputData& data = mouse_state[button];
					
					AR_CORE_INFO(event.dwEventFlags);

					if (data.is_down)
					{
						data.is_down = false;

						TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };
						MouseReleasedEvent e(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos);
						m_callback(e);
					}
				}
			}
			else if (event.dwEventFlags & MOUSE_MOVED)
			{
				Coord pos = getMousePos();
				TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };
				MouseMovedEvent e(pos, pos - m_mouse_pos, cur_pos, cur_pos - m_cur_pos);

				m_mouse_pos = pos;
				m_cur_pos = cur_pos;

				m_callback(e);
			}
		}
	}

	void WinEventListener::sendFocusEvent(FOCUS_EVENT_RECORD event)
	{
		TerminalFocusEvent e(event.bSetFocus);
		m_callback(e);
	}

	void WinEventListener::sendTermEvents()
	{
		// Resize event

		TermVert new_size = m_attr->terminalSize();

		if (new_size != m_last_term_size)
		{
			TerminalResizedEvent e(new_size, new_size - m_last_term_size, true);
			m_callback(e);
		}

		m_last_term_size = new_size;

		// Move event

		Coord new_pos = m_attr->terminalPos();

		if (new_pos != m_last_term_pos)
		{
			TerminalMovedEvent e(new_pos, new_pos - m_last_term_pos, true);
			m_callback(e);
		}

		m_last_term_pos = new_pos;
	}

	Coord EventListener::getGlobalMousePos()
	{
		POINT pos;
		AR_WIN_VERIFY(GetCursorPos(&pos));
		return pos;
	}

}
