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

		int getMouseCode()
		{
			return m_mouseCode;
		}

		EVENT_CATEGORY_DEFINE(CategoryInput| CategoryMouse)

	};

	class MousePressedEvent : public MouseKeyEvent
	{
	protected:
		bool m_pressed;

	protected:
		MousePressedEvent(int mouseCode, bool pressed)
			: MouseKeyEvent(mouseCode), m_pressed(pressed) {}

	public:

		EVENT_TYPE_DEFINE(MousePressed)
	};

	class MouseReleasedEvent : public MouseKeyEvent
	{
	protected:

	protected:
		MouseReleasedEvent(int mouseCode)
			: MouseKeyEvent(mouseCode) {}

	public:

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

		unsigned int getX() { return m_xpos; }
		unsigned int getY() { return m_ypos; }
		unsigned int getXDiff() { return m_xdiff; }
		unsigned int getYDiff() { return m_ydiff; }

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput, CategoryMouse)
	};

	class MouseScrolledEvent : public Event
	{
	protected:
		float m_xoff, m_yoff;
	public:
		MouseScrolledEvent(float xpos, float ypos)
			: m_xoff(xpos), m_yoff(ypos)
		{}

		unsigned int getXOff() { return m_xoff; }
		unsigned int getYOff() { return m_yoff; }

		EVENT_TYPE_DEFINE(MouseMoved)

		EVENT_CATEGORY_DEFINE(CategoryInput, CategoryMouse)
	};
}
