#pragma once

#include "Event.h"

namespace Asciir
{
	class KeyEvent : public Event
	{
	protected:
		int m_keyCode;

	protected:
		KeyEvent(int keyCode)
			:m_keyCode(keyCode)
		{}

		int getKeyCode() const { return m_keyCode; }


	public:
		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	class KeyPressedEvent : public KeyEvent
	{
		bool m_repeat;
	public:
		KeyPressedEvent(int keyCode, bool repeat)
			: KeyEvent(keyCode), m_repeat(repeat) {}

		std::string toString()
		{
			std::stringstream stream;

			stream << "KeyPressedEvent: \nCode: " << m_keyCode << ", Pressed: " << m_repeat;

			return stream.str();
		}

		EVENT_TYPE_DEFINE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string toString()
		{
			std::stringstream stream;

			stream << "KeyReleasedEvent: \nCode: " << m_keyCode;

			return stream.str();
		}

		EVENT_TYPE_DEFINE(KeyPressed)
	};
}