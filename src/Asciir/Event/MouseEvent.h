#pragma once

#include "Event.h"
#include "Asciir/Math/Vertices.h"

namespace Asciir
{
	class Input;

	class MouseKeyEvent : public Event
	{
	protected:
		MouseKey m_mousecode;
		Coord m_mouse_pos;
		TermVert m_cur_pos;

	protected:
		MouseKeyEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: m_mousecode(mousecode), m_mouse_pos(mouse_pos), m_cur_pos(cur_pos) {}

		MouseKeyEvent()
			: m_mousecode(MouseKey::UNKNOWN), m_mouse_pos(AR_INVALID_COORD), m_cur_pos(AR_INVALID_COORD) {
			m_valid = false;
		}

	public:

		MouseKey getMouseCode() const { AR_EVENT_IS_VALID; return m_mousecode; }
		Coord getMousePos() const { AR_EVENT_IS_VALID; return m_mouse_pos; }
		TermVert getCursorPos() const { AR_EVENT_IS_VALID; return m_cur_pos; }

		EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	class MousePressedEvent : public MouseKeyEvent
	{
	protected:
		bool m_double_click;
	public:
		MousePressedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos, bool double_click)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos), m_double_click(double_click) {}

		MousePressedEvent() : m_double_click(false) {}

		bool isDoubleClick() { AR_EVENT_IS_VALID; return m_double_click; }

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
	public:
		MouseReleasedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos) {}

		MouseReleasedEvent() {}

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseReleased: " << (int)m_mousecode;

			return msg.str();
		}

		EVENT_TYPE_DEFINE(MouseReleased)
	};

	class MouseMovedEvent : public Event
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

		MouseMovedEvent()
			: m_pos(AR_INVALID_COORD), m_diff(AR_INVALID_COORD), m_cur_diff(AR_INVALID_COORD) {
			m_valid = false;
		}

		Coord getPos() const { AR_EVENT_IS_VALID; return m_pos; }
		Coord getDiff() const { AR_EVENT_IS_VALID; return m_diff; }
		TermVert getCursorPos() const { AR_EVENT_IS_VALID; return m_cur_pos; }
		TermVert getCursorDiff() const { AR_EVENT_IS_VALID; return m_cur_diff; }

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

		MouseScrolledEvent()
			: m_off(AR_INVALID_COORD) {
			m_valid = false;
		}

		RealVertex getOff() const { AR_EVENT_IS_VALID; return m_off; }

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
