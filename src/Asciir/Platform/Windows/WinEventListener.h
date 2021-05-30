#pragma once

#include "Asciir/Input/EventListener.h"
#include "WindowsArAttributes.h"

namespace Asciir
{

	class WinEventListener : public EventListener
	{
	protected:

		std::thread m_input_thrd;
		DWORD m_hcin_fallback;
		HANDLE m_hConsole_in;
		HANDLE m_hConsole_out;

		Coord m_last_term_pos = {0, 0};
		TermVert m_last_term_size = {0, 0};

		const WinARAttr* m_attr = nullptr;

		static constexpr size_t BUFF_LEN = 256;
		INPUT_RECORD m_event_buffer[BUFF_LEN] = {0};

	public:
		WinEventListener();
		~WinEventListener() final;

		void start(EventCallbackFp callback) final;
		void stop() final;

		KeyInputData getKeybdFromWinCode(WORD code) const;
		KeyInputData getKeybdFromKeyCode(Key code) const;
		
		MouseInputData getMouseFromWinCode(WORD code) const;
		MouseInputData getMouseFromKeyCode(MouseKey code) const;

	protected:

		void listenForInputs();

		void sendKeybdEvent(KEY_EVENT_RECORD event);
		void sendMouseEvent(MOUSE_EVENT_RECORD event);
		void sendFocusEvent(FOCUS_EVENT_RECORD event);
		void sendTermEvents();

		std::atomic<bool> m_is_listening = false;
	};
}
