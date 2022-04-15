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

		std::thread m_idev_evt_thrd;
		std::thread m_win_evt_thrd;
		DWORD m_hcin_fallback;
		HANDLE m_hConsole_in;
		HANDLE m_hConsole_out;

		TermVert m_last_term_pos = { 0, 0 };
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
		KeyInputData& winGetKeyFromWinCode(WORD code);

		/// @brief returns input data about the given windows mouse key
		///
		/// @attention this function is windows specific, use getMouseKeyFromCode() for a cross platform alternative
		MouseInputData& winGetMouseKeyFromWinCode(WORD code);

		/// @brief retrieves the latest recieved terminal screen buffer resize event.
		TermVert winLastTermSize() const { return m_term_size; }

	protected:

		/// @brief thread responsible for listening for events input from devices (keyboard, gamepad, eg.), EXCEPT for a mouse input device.
		void iDeviceEvtThrd();
		/// @brief thread responsible for listening for events related to the terminal window (resize, window movement, eg.), as well as any mouse device inputs.
		void windowEvtThrd();

		/// @brief constructs a KeyEvent structure form the passed RAWKEYBOARD and passes it to the event handler.
		void sendKeybdEvent(RAWKEYBOARD event);
		/// @brief constructs a MouseEvent structure form the passed MOUSE_EVENT_RECORD and passes it to the event handler.
		void sendMouseEvent(MOUSE_EVENT_RECORD event);
		/// @brief constructs a FocusEvent structure form the passed FOCUS_EVENT_RECORD and passes it to the event handler.
		void sendFocusEvent(FOCUS_EVENT_RECORD event);
		/// @brief sends any terminal window related events to the event handler.
		///
		/// in contrast to the other send event methods, this one does not take any parameters,
		/// as it auto detects any changes related to the terminal status that has happened since the last call to this method.
		/// 
		/// it is done as such, as there is no way of detecting some events, like terminal window movement, without polling them.
		/// 
		void sendTermEvents();

		std::atomic<bool> m_is_listening = false;
		TermVert m_term_size;
	};
}

/// @brief typedef of the currently implemented EventListenerImpl interface @see EventListenerImpl<IMPLS::INTER>
typedef ELInterface::WinEventListener EventListener;

}
