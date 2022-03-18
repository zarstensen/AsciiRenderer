#include "pch/arpch.h"

#include "WinEventListener.h"
#include "KeyCodeMap.h"

#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Core/Application.h"

namespace Asciir
{
namespace ELInterface
{
	EventListenerImpl<IMPLS::WIN>::EventListenerImpl()
	{
		m_hConsole_in = GetStdHandle(STD_INPUT_HANDLE);
		m_hConsole_out = GetStdHandle(STD_OUTPUT_HANDLE);
		AR_WIN_VERIFY(m_hConsole_in);
		AR_WIN_VERIFY(m_hConsole_out);

		AR_WIN_VERIFY(GetConsoleMode(m_hConsole_in, &m_hcin_fallback));
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback & ~ENABLE_QUICK_EDIT_MODE | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT));
	}

	EventListenerImpl<IMPLS::WIN>::~EventListenerImpl()
	{
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback));
	}

	void EventListenerImpl<IMPLS::WIN>::start(EventCallbackFp callback)
	{
		m_trenderer = &ARApp::getApplication()->getTermRenderer();

		m_last_term_pos = m_trenderer->pos();
		m_last_term_size = m_trenderer->termSize();

		m_callback = callback;
		m_is_listening = true;
		m_input_thrd = std::thread(&EventListenerImpl<IMPLS::WIN>::listenForInputs, this);
	}

	void EventListenerImpl<IMPLS::WIN>::stop()
	{
		m_is_listening = false;
		m_input_thrd.join();
	}

	EventListenerImpl<IMPLS::WIN>::KeyInputData EventListenerImpl<IMPLS::WIN>::getKeyFromWinCode(WORD code) const
	{
		return getKeyFromCode(WinToKeyCodeMap.at(code));
	}

	EventListenerImpl<IMPLS::WIN>::MouseInputData EventListenerImpl<IMPLS::WIN>::getMouseKeyFromWinCode(WORD code) const
	{
		return getMouseKeyFromCode(WinToMouseCodeMap.at(code));
	}

	void EventListenerImpl<IMPLS::WIN>::listenForInputs()
	{
		while (m_is_listening)
		{
			DWORD num_events;
			AR_WIN_VERIFY(GetNumberOfConsoleInputEvents(m_hConsole_in, &num_events));

			if (num_events > 0)
			{
				DWORD num_read;
				AR_WIN_VERIFY(ReadConsoleInput(m_hConsole_in, m_event_buffer, BUFF_LEN, &num_read));

				CONSOLE_SCREEN_BUFFER_INFOEX test;
				test.cbSize = sizeof(test);

				AR_WIN_VERIFY(GetConsoleScreenBufferInfoEx(m_hConsole_out, &test));

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

	void EventListenerImpl<IMPLS::WIN>::sendKeybdEvent(KEY_EVENT_RECORD event)
	{
	#ifdef AR_SAFE_RELEASE
		if (WinToKeyCodeMap.count(event.wVirtualKeyCode) == 0)
		{
			AR_CORE_WARN("Unknown key recieved: ", event.wVirtualKeyCode);
			return;
		}
	#endif

		if (event.bKeyDown)
		{
			KeyInputData& data = keybd_state[(size_t)WinToKeyCodeMap.at(event.wVirtualKeyCode) - 1];
			KeyPressedEvent e;

			if (!data.is_down)
			{
				data.is_repeat = true;
				data.is_down = true;
				data.time_since_down = getTime();

				e = KeyPressedEvent(WinToKeyCodeMap.at(event.wVirtualKeyCode), false);

				m_callback(e);

				for (size_t i = 0; i < event.wRepeatCount - 1; i++)
				{
					e = KeyPressedEvent(WinToKeyCodeMap.at(event.wVirtualKeyCode), true);

					m_callback(e);
				}
			}
			else if (data.is_down)
			{
				data.is_repeat = false;

				for (size_t i = 0; i < event.wRepeatCount; i++)
				{
					e = KeyPressedEvent(WinToKeyCodeMap.at(event.wVirtualKeyCode), true);

					m_callback(e);
				}
			}

			// store down event
			keybd_down_state[(size_t)WinToKeyCodeMap.at(event.wVirtualKeyCode) - 1] = data;
		}
		else
		{
			KeyInputData& data = keybd_state[(size_t)WinToKeyCodeMap.at(event.wVirtualKeyCode) - 1];
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
	void EventListenerImpl<IMPLS::WIN>::sendMouseEvent(MOUSE_EVENT_RECORD event)
	{
		for (int button = 0; button < 5; button++)
		{
			WORD button_word = (WORD)BIT_SHL(button);

			if (event.dwEventFlags & DOUBLE_CLICK)
			{
				if (CHECK_BIT(event.dwButtonState, button))
				{
					MouseInputData& data = mouse_state[button];
					data.is_double_click = true;

					TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };

					MousePressedEvent e_first(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos, false);
					MousePressedEvent e_second(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos, true);

					m_callback(e_first);
					m_callback(e_second);

					// store down event
					mouse_down_state[button] = data;
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

						// store down event
						mouse_down_state[button] = data;
					}
				}
				else
				{
					MouseInputData& data = mouse_state[button];

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
				Coord pos = getCurrentMousePos(); // get the current position of the mouse
				
				TermVert cur_pos = { event.dwMousePosition.X, event.dwMousePosition.Y };


				// setup event data from the last position and the current position of the mouse
				MouseMovedEvent e(pos, pos - m_mouse_pos, cur_pos, cur_pos - m_cur_pos);

				// current mouse position is stored for the next event
				m_mouse_pos = pos;
				m_cur_pos = cur_pos;

				// pass the generated event to the callback
				m_callback(e);
			}
		}
	}

	void EventListenerImpl<IMPLS::WIN>::sendFocusEvent(FOCUS_EVENT_RECORD event)
	{
		TerminalFocusEvent e(event.bSetFocus);
		m_callback(e);
	}

	void EventListenerImpl<IMPLS::WIN>::sendTermEvents()
	{
		// Resize event

		TermVert new_size = m_trenderer->termSize();

		if (new_size != m_last_term_size)
		{
			TerminalResizedEvent e(new_size, new_size - m_last_term_size, true);
			m_callback(e);
		}

		m_last_term_size = new_size;

		// Move event

		Coord new_pos = m_trenderer->pos();

		if (new_pos != m_last_term_pos)
		{
			TerminalMovedEvent e(new_pos, new_pos - m_last_term_pos, true);
			m_callback(e);
		}

		m_last_term_pos = new_pos;
	}

	Coord EventListenerImpl<IMPLS::INTER>::getCurrentMousePos()
	{
		POINT pos;
		AR_WIN_VERIFY(GetCursorPos(&pos));
		return pos;
	}
}
}