#pragma once

#include "Event.h"

namespace Asciir
{
	class MouseKeyEvent: public Event
	{
	protected:
		int m_mouseCode;

	protected:
		MouseKeyEvent(int mouseCode)
			: m_mouseCode(mouseCode) {}

	public:

		int getMouseCode() const { return m_mouseCode; }

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)

	};

	class MousePressedEvent : public MouseKeyEvent
	{
	protected:
		bool m_pressed;

	protected:
		MousePressedEvent(int mouseCode, bool pressed)
			: MouseKeyEvent(mouseCode), m_pressed(pressed) {}

	public:

		bool isPressed() const { return m_pressed; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseReleased: " << getMouseCode() << ',' << isPressed();

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MousePressed)
	};

	class MouseReleasedEvent : public MouseKeyEvent
	{
	protected:
		MouseReleasedEvent(int mouseCode)
			: MouseKeyEvent(mouseCode) {}

	public:

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseReleased: " << getMouseCode();

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

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};
}
