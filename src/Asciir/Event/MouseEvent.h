#pragma once

#include "Event.h"

namespace Asciir
{

	class Input;

	class MouseKeyEvent: public Event
	{
	protected:
		MouseKey m_mousecode;

	protected:
		MouseKeyEvent(MouseKey mousecode)
			: m_mousecode(mousecode) {}

	public:

		MouseKey getMouseCode() const { return m_mousecode; }

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)

	};

	class MousePressedEvent : public MouseKeyEvent
	{
		friend Input;
	protected:
		MousePressedEvent(MouseKey mousecode)
			: MouseKeyEvent(mousecode) {}

	public:

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MousePressed: " << (int)m_mousecode;

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MousePressed)
	};

	class MouseReleasedEvent : public MouseKeyEvent
	{
		friend Input;
	protected:
		MouseReleasedEvent(MouseKey mousecode)
			: MouseKeyEvent(mousecode) {}

	public:

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseReleased: " << (int) m_mousecode;

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseReleased)
	};

	class MouseMovedEvent: public Event
	{
	protected:
		Coord m_pos;
		Coord m_diff;
	public:
		MouseMovedEvent(Coord pos, Coord diff)
			: m_pos(pos), m_diff(diff)
		{}

		Coord getPos() const { return m_pos; }
		Coord getDiff() const { return m_diff; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseMoved: " << getPos() << ':' << getDiff();
			
			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	class MouseScrolledEvent : public Event
	{
	protected:
		RealVertex m_off;
	public:
		MouseScrolledEvent(RealVertex off)
			: m_off(off)
		{}

		RealVertex getOff() const { return m_off; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseScrolled: " << getOff();

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseScrolled)

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};
}
