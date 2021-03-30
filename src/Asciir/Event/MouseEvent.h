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
		unsigned int m_xpos, m_ypos;
		unsigned int m_xdiff, m_ydiff;
	public:
		MouseMovedEvent(unsigned int xpos, unsigned int ypos, unsigned int xdiff, unsigned int ydiff)
			: m_xpos(xpos), m_ypos(ypos), m_xdiff(xdiff), m_ydiff(ydiff)
		{}

		unsigned int getX() const { return m_xpos; }
		unsigned int getY() const { return m_ypos; }
		unsigned int getXDiff() const { return m_xdiff; }
		unsigned int getYDiff() const { return m_ydiff; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseMoved: " << getX() << ',' << getY() << ':' << getXDiff() << ',' << getYDiff();
			
			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	class MouseScrolledEvent : public Event
	{
	protected:
		float m_xoff, m_yoff;
	public:
		MouseScrolledEvent(float xpos, float ypos)
			: m_xoff(xpos), m_yoff(ypos)
		{}

		float getXOff() const { return m_xoff; }
		float getYOff() const { return m_yoff; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseScrolled: " << getXOff() << ',' << getYOff();

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};
}
