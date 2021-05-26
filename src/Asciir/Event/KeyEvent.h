#pragma once

#include "Event.h"
#include "Asciir/Input/KeyCodes.h"

namespace Asciir
{
	class KeyEvent : public Event
	{
	protected:
		Key m_keycode;

	protected:
		KeyEvent(Key keycode)
			:m_keycode(keycode)
		{}

		KeyEvent()
			: m_keycode(Key::UNKNOWN) { m_valid = false; }

		Key getKeyCode() const { AR_EVENT_IS_VALID; return m_keycode; }


	public:
		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	class KeyPressedEvent : public KeyEvent
	{
		bool m_repeat;
	public:
		KeyPressedEvent(Key keycode, bool repeat)
			: KeyEvent(keycode), m_repeat(repeat) {}

		KeyPressedEvent()
			: m_repeat(false) {}

		std::string toString() const override
		{
			std::stringstream stream;

			stream << "KeyPressedEvent: \nCode: " << (int)m_keycode << ", Pressed: " << m_repeat;

			return stream.str();
		}

		EVENT_TYPE_DEFINE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key keycode)
			: KeyEvent(keycode) {}

		KeyReleasedEvent() {}

		std::string toString() const override
		{
			std::stringstream stream;

			stream << "KeyReleasedEvent: \nCode: " << (int)m_keycode;

			return stream.str();
		}

		EVENT_TYPE_DEFINE(KeyReleased)
	};
}