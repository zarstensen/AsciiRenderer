#include "pch/arpch.h"

#include "WinEventListener.h"
#include "KeyCodeMap.h"
#include "WinTermRenderer.h"

#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Core/Application.h"

#include "Asciir/Rendering/Renderer.h"

#include <WinUser.h>
#include <hidusage.h>

namespace Asciir
{
namespace ELInterface
{
	WinEventListener::WinEventListener()
	{
		m_hConsole_in = GetStdHandle(STD_INPUT_HANDLE);
		m_hConsole_out = GetStdHandle(STD_OUTPUT_HANDLE);
		AR_WIN_VERIFY(m_hConsole_in);
		AR_WIN_VERIFY(m_hConsole_out);

		AR_WIN_VERIFY(GetConsoleMode(m_hConsole_in, &m_hcin_fallback));
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback & ~ENABLE_QUICK_EDIT_MODE | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT));
		
		// get the size of the terminal

		CONSOLE_SCREEN_BUFFER_INFOEX buff_info{ sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };

		AR_WIN_VERIFY(GetConsoleScreenBufferInfoEx(m_hConsole_out, &buff_info));

		m_term_size = TermVert(buff_info.dwSize.X, buff_info.dwSize.Y);

	}

	WinEventListener::~WinEventListener()
	{
		AR_WIN_VERIFY(SetConsoleMode(m_hConsole_in, m_hcin_fallback));
	}

	void WinEventListener::start(EventCallbackFp callback)
	{
		m_trenderer = &ARApp::getApplication()->getTermRenderer();

		m_last_term_pos = m_trenderer->pos();
		m_last_term_size = m_trenderer->termSize();

		
		// start thread listeners

		m_callback = callback;
		m_is_listening = true;

		m_idev_evt_thrd = std::thread(&WinEventListener::windowEvtThrd, this);
		m_win_evt_thrd = std::thread(&WinEventListener::iDeviceEvtThrd, this);
	}

	void WinEventListener::stop()
	{
		m_is_listening = false;

		// make sure the message loop is stopped before the input threads are joined
		SendMessage(NULL, WM_QUIT, NULL, NULL);

		m_idev_evt_thrd.join();
		m_win_evt_thrd.join();
	}
	
	WinEventListener::KeyInputData& WinEventListener::winGetKeyFromWinCode(WORD code)
	{
		return keybd_state[(size_t)WinToKeyCodeMap.at(code) - 1];
	}

	WinEventListener::MouseInputData& WinEventListener::winGetMouseKeyFromWinCode(WORD code)
	{
		return mouse_state[(size_t)WinToMouseCodeMap.at(code) - 1];
	}

	void WinEventListener::iDeviceEvtThrd()
	{
		// setup Windows message only window

		WNDCLASSEX wndclass = {};
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.lpfnWndProc = DefWindowProc;
		wndclass.hInstance = GetModuleHandle(NULL);
		wndclass.lpszClassName = TEXT("AsciirInputWindow");

		AR_WIN_VERIFY(RegisterClassEx(&wndclass));

		HWND message_window = CreateWindowEx(NULL, wndclass.lpszClassName, NULL, NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, wndclass.hInstance, NULL);

		AR_WIN_VERIFY(message_window);

		// setup raw input devices

		// TODO: this should be able to be modified at runtime, so other input devices, such as controllers, can be added.

		std::array<RAWINPUTDEVICE, 1> rid;

		// keyboard device

		rid[0] = RAWINPUTDEVICE{ HID_USAGE_PAGE_GENERIC , HID_USAGE_GENERIC_KEYBOARD, RIDEV_INPUTSINK | RIDEV_NOLEGACY, message_window };

		AR_WIN_VERIFY(RegisterRawInputDevices(rid.data(), (UINT) rid.size(), sizeof(RAWINPUTDEVICE)));

		// begin message loop

		MSG msg;

		while (m_is_listening && GetMessage(&msg, NULL, NULL, NULL))
		{
			if (msg.message == WM_INPUT) // recieved input from a device
			{
				HRAWINPUT hrinp = (HRAWINPUT)msg.lParam;

				UINT cb_size;

				GetRawInputData(hrinp, RID_INPUT, NULL, &cb_size, sizeof(RAWINPUTHEADER));

				RAWINPUT* rinp = (RAWINPUT*)new uint8_t[cb_size];

				GetRawInputData(hrinp, RID_INPUT, rinp, &cb_size, sizeof(RAWINPUTHEADER));

				switch (rinp->header.dwType)
				{
				case RIM_TYPEMOUSE:
					// raw mouse movement is very unpractical to handle, as it in most cases ONLY gives relative position changes, making the initial position impossible to deduce.
					// it also does not take into account mouse acceleration.
					break;
				case RIM_TYPEKEYBOARD:
					sendKeybdEvent(rinp->data.keyboard);
					break;
				case RIM_TYPEHID:
					AR_ASSERT_MSG(false, "Custom input devices are not yet supported")
					break;
				}
			}
			else
			{
				AR_INFO("UNKNOWN MSG: ", msg.message);
			}
		}
		
		// cleanup

		AR_WIN_VERIFY(UnregisterClass(TEXT("AsciirInputWindow"), GetModuleHandle(NULL)));
		AR_WIN_VERIFY(DestroyWindow(message_window));
	}

	void WinEventListener::windowEvtThrd()
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
					// device input events are now handlet by the iDeviceEvtTHrd
					case KEY_EVENT:
						break;
					case MOUSE_EVENT:
						sendMouseEvent(event_r.Event.MouseEvent);
						break;
					// END
					case FOCUS_EVENT:
						sendFocusEvent(event_r.Event.FocusEvent);
						break;
					case WINDOW_BUFFER_SIZE_EVENT:
						COORD size = event_r.Event.WindowBufferSizeEvent.dwSize;
						m_term_size = { size.X, size.Y };
						break;
					case MENU_EVENT:
						break;
					default:
						// this should never be hit
						AR_CORE_WARN("Unknown windows console event type: ", m_event_buffer[i].EventType);
						break;
					}
				}
			}

			sendTermEvents();
		}
	}

	void WinEventListener::sendKeybdEvent(RAWKEYBOARD event)
	{
	#ifdef AR_SAFE_RELEASE
		if (WinToKeyCodeMap.count(event.VKey) == 0)
		{
			AR_CORE_WARN("Unknown key recieved: ", event.VKey);
			return;
		}
	#endif
		if ((event.Flags & 0b1) == RI_KEY_MAKE)
		{
			KeyInputData& data = winGetKeyFromWinCode(event.VKey);
			KeyPressedEvent e;

			e = KeyPressedEvent(WinToKeyCodeMap.at(event.VKey), data.is_repeat);
				
			data.is_repeat = true;
			data.is_down = true;
			data.time_since_down = getTime();

			m_callback(e);
		}
		else
		{
			KeyInputData& data = winGetKeyFromWinCode(event.VKey);
			KeyReleasedEvent e(WinToKeyCodeMap.at(event.VKey));
			
			data.is_repeat = false;
			data.is_down = false;

			m_callback(e);
		}
	}

	// mouse pos on the screen is found when the event is recieved, so it might be slightly off compared to when the event was sent
	void WinEventListener::sendMouseEvent(MOUSE_EVENT_RECORD event)
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

					MousePressedEvent e_double(WinToMouseCodeMap.at(button_word), getMousePos(), cur_pos, true);

					m_callback(e_double);

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
				TermVert pos = getCurrentMousePos(); // get the current position of the mouse

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

	void WinEventListener::sendFocusEvent(FOCUS_EVENT_RECORD event)
	{
		TerminalFocusEvent e(event.bSetFocus);
		m_callback(e);
	}

	void WinEventListener::sendTermEvents()
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

		TermVert new_pos = m_trenderer->pos();

		if (new_pos != m_last_term_pos)
		{
			TerminalMovedEvent e(new_pos, new_pos - m_last_term_pos, true);
			m_callback(e);
		}

		m_last_term_pos = new_pos;
	}

	TermVert EventListenerImpl::getCurrentMousePos()
	{
		POINT pos;
		AR_WIN_VERIFY(GetCursorPos(&pos));
		return pos;
	}
}
}
