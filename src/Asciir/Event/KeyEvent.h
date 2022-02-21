#pragma once

#include "Event.h"
#include "Asciir/Input/KeyCodes.h"

namespace Asciir
{
	/// @brief base event class for all key events
	/// holds the keycode that generated the event
	/// 
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryKeyboard  
	/// Type: None
	/// 
	class KeyEvent : public Event
	{
	protected:
		Key m_keycode;

		/// @param keycode the keycode that generated the key event
		KeyEvent(Key keycode)
			:m_keycode(keycode)
		{}

		KeyEvent()
			: m_keycode(Key::UNKNOWN) {
			m_valid = false;
		}

	public:
		/// @return keycode that generated the event
		Key getKeyCode() const { AR_EVENT_IS_VALID; return m_keycode; }

		AR_EVENT_CATEGORY_DEFINE(CategoryInput | CategoryKeyboard)
	};

	/// @brief event class for key pressed events
	/// 
	/// stores the keycode that generated the event, as well as the repeat flag for the key event.
	/// the repeat flag is set to true if the keycode was generated because it was held down for an extended period of time.
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryKeyboard  
	/// Type: EventType::KeyPressed  
	/// 
	class KeyPressedEvent : public KeyEvent
	{
		bool m_repeat;
	public:
		KeyPressedEvent(Key keycode, bool repeat)
			: KeyEvent(keycode), m_repeat(repeat) {}

		KeyPressedEvent()
			: m_repeat(false) {}

		/// @return returns true if the key event was generated due to the key being held down.
		bool isRepeat() { AR_EVENT_IS_VALID; return m_repeat; }

		/// @brief convert event to string  
		/// Example:  
		/// > KeyPressedEvent:  
		/// > Code: 0, Pressed: 0  
		/// @return the name of the event, the keycode and the repeat flag
		std::string toString() const override
		{
			std::stringstream stream;

			stream << "KeyPressedEvent: \nCode: " << (int)m_keycode << ", Pressed: " << m_repeat;

			return stream.str();
		}

		AR_EVENT_TYPE_DEFINE(KeyPressed)
	};

	/// @brief class for key released events
	///
	/// stores the keycode of the key release event
	///
	/// Categories: Asciir::CategoryInput, Asciir::CategoryKeyboard  
	/// Type: EventType::KeyReleased
	/// 
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key keycode)
			: KeyEvent(keycode) {}

		KeyReleasedEvent() {}

		/// @brief convert event to string  
		/// Example:  
		/// > KeyReleasedEvent:  
		/// > Code: 0
		/// @return the name of the event and the keycode
		std::string toString() const override
		{
			std::stringstream stream;

			stream << "KeyReleasedEvent: \nCode: " << (int)m_keycode;

			return stream.str();
		}

		AR_EVENT_TYPE_DEFINE(KeyReleased)
	};
}