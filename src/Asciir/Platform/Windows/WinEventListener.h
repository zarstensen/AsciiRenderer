#pragma once

#include "Asciir/Input/EventListener.h"
#include "Asciir/Rendering/TerminalRenderer.h"

namespace Asciir
{
namespace ELInterface
{
	/// @brief windows specific implementation of the EventListener class
	/// @see EventListenerImpl<IMPLS::INTER>
	class WinEventListener : public EventListenerImpl
	{
	protected:

		std::thread m_input_thrd;
		DWORD m_hcin_fallback;
		HANDLE m_hConsole_in;
		HANDLE m_hConsole_out;

		Coord m_last_term_pos = { 0, 0 };
		TermVert m_last_term_size = { 0, 0 };

		const TerminalRenderer* m_trenderer = nullptr;

		static constexpr size_t BUFF_LEN = 256;
		INPUT_RECORD m_event_buffer[BUFF_LEN] = { 0 };

	public:
		/// @brief sets up the windows terminal
		WinEventListener();
		/// @brief rolls back the windows terminal, so that any chagnes done to the terminal, are undone
		~WinEventListener();

		/// @brief sets initial values and starts the event listener thread
		/// @see EventListenerImpl<IMPLS::INTER>::start()
		void start(EventCallbackFp callback); // compiletime override
		/// @brief stops the event listener thread
		/// @see EventListenerImpl<IMPLS::INTER>::stop()
		void stop(); // compiletime override

		/// @brief gives information back about the given windows [Virtual-Key Code](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)
		/// 
		/// @attention this function is windows specific, use getKeyFromCode() for a cross platform alternative
		KeyInputData winGetKeyFromWinCode(WORD code) const;

		/// @brief returns input data about the given windows mouse key
		///
		/// @attention this function is windows specific, use getMouseKeyFromCode() for a cross platform alternative
		MouseInputData winGetMouseKeyFromWinCode(WORD code) const;

		/// @brief retrieves the latest recieved terminal screen buffer resize event.
		TermVert winLastTermSize() const { return m_term_size; }

	protected:

		void listenForInputs();

		void sendKeybdEvent(KEY_EVENT_RECORD event);
		void sendMouseEvent(MOUSE_EVENT_RECORD event);
		void sendFocusEvent(FOCUS_EVENT_RECORD event);
		void sendTermEvents();

		std::atomic<bool> m_is_listening = false;
		TermVert m_term_size;
	};
}

/// @brief typedef of the currently implemented EventListenerImpl interface @see EventListenerImpl<IMPLS::INTER>
typedef ELInterface::WinEventListener EventListener;

}