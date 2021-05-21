#pragma once

#include "Event.h"

namespace Asciir
{

	class Input;

	class MouseKeyEvent: public Event
	{
	protected:
		MouseKey m_mousecode;
		Coord m_mouse_pos;
		TermVert m_cur_pos;

	protected:
		MouseKeyEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: m_mousecode(mousecode), m_mouse_pos(mouse_pos), m_cur_pos(cur_pos) {}

	public:

		MouseKey getMouseCode() const { return m_mousecode; }
		Coord getMousePos() const { return m_mouse_pos;  }
		TermVert getCursorPos() const { return m_cur_pos; }

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)

	};

	class MousePressedEvent : public MouseKeyEvent
	{
		friend Input;
	protected:
		MousePressedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos) {}

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
		MouseReleasedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos) {}

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
		TermVert m_cur_pos;
		TermVert m_cur_diff;

	public:
		MouseMovedEvent(Coord pos, Coord diff, TermVert cur_pos, TermVert cur_diff)
			: m_pos(pos), m_diff(diff), m_cur_pos(cur_pos), m_cur_diff(cur_diff)
		{}

		Coord getPos() const { return m_pos; }
		Coord getDiff() const { return m_diff; }
		TermVert getCursorPos() const { return m_cur_pos; }
		TermVert getCursorDiff() const { return m_cur_diff;  }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseMoved: " << getPos() << ':' << getDiff() << "   " << getCursorPos() << ':' << getCursorDiff();
			
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
