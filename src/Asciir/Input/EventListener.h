#pragma once

#include "Asciir/Event/Event.h"
#include "Asciir/Input/KeyCodes.h"

namespace Asciir
{
	namespace ELInterface // short for EventListenerInterface
	{
		/// @brief enum for the different possible implementations of the EventListener interface
		enum class IMPLS
		{
			INTER, // interface version, all other implementations should inherit from this
			WIN,
			MAC,
			LINUX
		};

		/// @brief interface class for the Asciir::EventListener type
		template<IMPLS>
		class EventListenerImpl;

		/// @brief interface declaration for the EventListener interface, will contain no definitions,
		/// as this class should be inherited by an implementation, where this is implemented as the interface
		/// 
		/// use start() and stop() to control when the eventlistener is active.
		/// 
		/// the callback passed to start() will recieve any console input events (mouse, keyboard, focus)
		/// 
		/// the eventlistener also has a poll input interface.
		/// use pollState() to poll the current input states of different input devices (mouse, keyboard).
		/// this polled data can be accessed through getKeybdPoll() and getMousePoll()
		/// 
		/// @attention this class should never be instantiated and should only be used as a base class for other implementations.  
		/// to get the current implementation type, use the typedef EventListener
		template<>
		class EventListenerImpl<IMPLS::INTER>
		{
		public:
			using EventCallbackFp = std::function<void(Event&)>;

			/// @brief structures that holds key down state and detection time
			struct KeyInputData
			{
				bool is_down = false;
				bool is_repeat = false;

				duration time_since_down = duration();
			};

			/// @brief structure that holds mouse down state and detection time
			struct MouseInputData
			{
				bool is_down = false;
				bool is_double_click = false;

				duration time_since_down = duration();
			};

			/// @brief size of key input array for the keyboard
			static constexpr size_t KIS_LEN = KEY_CODE_COUNT;
			/// @brief size of key input array for the mouse
			static constexpr size_t MIS_LEN = MOUSE_CODE_COUNT;

		protected:
			EventCallbackFp m_callback;

			/// @brief buffer responsible for storing the current state of the keyboard, based on events.
			// updated for every event.
			std::array<KeyInputData, KIS_LEN> keybd_state;
			/// @brief buffer that only stores key down events.
			/// used to make sure any keydown events always will be polled, even if the key is up on the poll call.
			/// gets cleared every call to pollState.
			std::array<KeyInputData, KIS_LEN> keybd_down_state;
			// @brief buffer copies and stores keybd_down_state every pollUpdate
			std::array<KeyInputData, KIS_LEN> keybd_poll_state;

			/// @brief same as keybd_state but for mouse data
			std::array<MouseInputData, MIS_LEN> mouse_state;
			/// @brief same as keybd_down_state but for mouse data
			std::array<MouseInputData, MIS_LEN> mouse_down_state;
			/// @brief same as keybd_poll_state but for mouse data
			std::array<MouseInputData, MIS_LEN> mouse_poll_state;

			Coord m_mouse_pos;
			Coord m_poll_mouse_pos;
			TermVert m_poll_cur_pos;
			TermVert m_cur_pos;

		public:

			EventListenerImpl() {};
			~EventListenerImpl() {};

			/// @brief starts the eventlistener.
			/// @param callback fucntion to recieve any events after eventlistener has started.
			void start(EventCallbackFp callback);
			/// @brief stops the eventlistener, the callback will no longer recieve any events.
			void stop();

			/// @brief polls the current input state for the mouse and keyboard.  
			/// any keydown events inbetween polls will be detected as keydown events, even if the key is up by the time pollState() is called.
			/// use getKeybdPoll() and getMousePoll() to get the polled data
			void pollState()
			{
				keybd_poll_state = keybd_down_state;
				mouse_poll_state = mouse_down_state;
				keybd_down_state = keybd_state;
				mouse_down_state = mouse_state;
			};

			/// @return returns the latest keyboard poll as an array, which stores all the individual key states of keys on the keyboard.  
			/// the index for a specific key, can be calculated as  
			/// i = @link Key Key @endlink - 1
			/// @see pollState
			const std::array<KeyInputData, KIS_LEN>& getKeybdPoll() const { return keybd_poll_state; }
			/// @return returns the latest mouse poll as an array, which stores all the individual mouse key states of mouse keys on the mouse.  
			/// the index for a specific mouse key, can be calculated as  
			/// i = @link MouseKey MouseKey @endlink - 1
			/// @see pollState
			const std::array<MouseInputData, MIS_LEN>& getMousePoll() const { return mouse_poll_state; }

			/// @brief return the latest polled key data for the given key code
			const KeyInputData& getKeyFromCode(Key code) const { return getKeybdPoll()[(size_t)code - 1]; }
			/// @brief return the latest polled mouse key data for the given mouse key code
			const MouseInputData& getMouseKeyFromCode(MouseKey code) const { return getMousePoll()[(size_t)code - 1]; }

			/// @return the mouse position at the time of the latest pollState() call. 
			Coord getMousePosPoll() { return m_poll_mouse_pos; };
			/// @return the mouse position recieved from the latest mouse moved event. 
			Coord getMousePos() { return m_mouse_pos; };

			/// @return the cursor position at the time of the latest pollState() call.
			/// @note cursor refers to the terminal column and row the mouse cursor is inside
			TermVert getCursorPosPoll() { return m_poll_cur_pos; };
			/// @return the current cursor position.  
			/// @note cursor refers to the terminal column and row the mouse cursor is inside
			TermVert getCursorPos() { return m_cur_pos; };
			/// @brief prompts the system for the current mouseposition,
			/// should return the same as getMousePos() if the pointer is inside the terminal window, else returns the position of the pointer outside the terminal window.
			static Coord getCurrentMousePos();
		};
	}
} // end of Asciir namespace

#ifdef AR_WIN
	#include "Asciir/Platform/Windows/WinEventListener.h"
	namespace Asciir
	{
		/// @brief typedef of the currently implemented EventListenerImpl interface @see EventListenerImpl<IMPLS::INTER>
		typedef ELInterface::EventListenerImpl<ELInterface::IMPLS::WIN> EventListener;
	}
#elif defined(AR_MAC)
#elif defined(AR_LINUX)
#else
#error platform not specified, unable to decide on interface implementaion for EventListener
#endif
