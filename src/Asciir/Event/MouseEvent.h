#pragma once

#include "Event.h"
#include "Asciir/Maths/Vertices.h"

namespace Asciir
{
	/// @brief base event class for all mouse key events
	/// 
	/// holds the mouse keycode that generated the event
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryMouse  
	/// Type: None
	///
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
			: m_mousecode(MouseKey::UNKNOWN), m_mouse_pos(INVALID_EVENT_COORD), m_cur_pos(INVALID_EVENT_COORD) {
			m_valid = false;
		}

	public:

		/// @return mouse key code that generated the event
		MouseKey getMouseCode() const { AR_EVENT_IS_VALID; return m_mousecode; }
		/// @return returns the coordinates of the mouse on the screen, relative to the top right corner of the monitor, when the event occured*
		/// @note * the actual coordinates are of the mouse when the event was recieved, instead of generated, due to limitations in getting the mouse coordinate of a terminal click event.
		Coord getPos() const { AR_EVENT_IS_VALID; return m_mouse_pos; }
		/// @brief returns the tile and column of the terminal, relative to the top right corner of the terminal application, when the event occured
		/// 
		/// @note in this case, this position is the position when the event occured, in contrast to getMousePos()
		/// @return column and row the mouse clicked on.
		TermVert getCursorPos() const { AR_EVENT_IS_VALID; return m_cur_pos; }

		AR_EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	/// @brief event class for mouse pressed events
	/// 
	/// stores the mouse keycode that generated the event, as well as wether the event is a double click.
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryMouse  
	/// Type: EventType::MousePressed  
	/// 
	class MousePressedEvent : public MouseKeyEvent
	{
	protected:
		bool m_double_click;
	public:
		MousePressedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos, bool double_click)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos), m_double_click(double_click) {}

		MousePressedEvent() : m_double_click(false) {}

		/// @return wether the mouse click event is a doubleclick
		bool isDoubleClick() { AR_EVENT_IS_VALID; return m_double_click; }

		/// @return string will be formatted as: "MousePressed: {getMouseCode()} : {getPos()}"
		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MousePressed: " << (int)m_mousecode << " : " << getPos();

			return msg.str();
		}

		AR_EVENT_TYPE_DEFINE(MousePressed)
	};

	/// @brief event class for mouse released events
	/// 
	/// stores the mouse keycode that generated the event.
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryMouse  
	/// Type: EventType::MouseReleased  
	/// 
	class MouseReleasedEvent : public MouseKeyEvent
	{
	public:
		MouseReleasedEvent(MouseKey mousecode, Coord mouse_pos, TermVert cur_pos)
			: MouseKeyEvent(mousecode, mouse_pos, cur_pos) {}

		MouseReleasedEvent() {}

		/// @return string will be formatted as: "MouseReleased: {getMousecode()} : {getPos()}"
		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseReleased: " << (int)m_mousecode << " : " << getPos();

			return msg.str();
		}

		AR_EVENT_TYPE_DEFINE(MouseReleased)
	};

	/// @brief event class for mouse movement released events
	/// 
	/// stores various information about the mouse movement.
	/// 
	/// @see getPos()  getDiff()  getCursorPos()  getCursorDiff()
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryMouse  
	/// Type: EventType::MouseMoved  
	/// 
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
			: m_pos(INVALID_EVENT_COORD), m_diff(INVALID_EVENT_COORD), m_cur_diff(INVALID_EVENT_COORD) {
			m_valid = false;
		}

		/// @return returns the position of the mouse when it generated its mouse moved event
		/// @note has the same limitation as MouseKeyEvent::getPos() (see the note)
		Coord getPos() const { AR_EVENT_IS_VALID; return m_pos; }
		/// @return returns the delta position of the mosue compared to the last mouse moved event.
		Coord getDiff() const { AR_EVENT_IS_VALID; return m_diff; }
		/// @return returns the column and the row the mouse is located inside, when it generated the mouse moved event
		TermVert getCursorPos() const { AR_EVENT_IS_VALID; return m_cur_pos; }
		/// @return returns the delta version of getCursorPos(), same logic as getDiff()
		TermVert getCursorDiff() const { AR_EVENT_IS_VALID; return m_cur_diff; }

		/// @return formatted as: "MouseMoved: {getPos()}:{getDiff()}   {getCursorPos()}:{getCursorDiff()}"
		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseMoved:\n" << getPos() << ':' << getDiff() << "   " << getCursorPos() << ':' << getCursorDiff();

			return msg.str();
		}

		AR_EVENT_TYPE_DEFINE(MouseMoved)

		AR_EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};

	/// @brief event class for mouse scroll released events
	/// 
	/// stores various information about the mouse scroll.
	/// 
	/// @see getOff()
	/// 
	/// Categories: Asciir::CategoryInput, Asciir::CategoryMouse  
	/// Type: EventType::MouseScrolled  
	/// 
	class MouseScrolledEvent : public Event
	{
	protected:
		RealVertex m_off;
	public:
		MouseScrolledEvent(RealVertex off)
			: m_off(off)
		{}

		MouseScrolledEvent()
			: m_off(INVALID_EVENT_COORD) {
			m_valid = false;
		}

		/// @return returns the delta of the mouse scroll (<0 -> down  :  >0 -> up)
		RealVertex getOff() const { AR_EVENT_IS_VALID; return m_off; }

		/// @return formatted as "MouseScrolled: {getOff()}" 
		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "MouseScrolled: " << getOff();

			return msg.str();
		}

		AR_EVENT_TYPE_DEFINE(MouseScrolled)

		AR_EVENT_CATEGORY_DEFINE(CategoryInput | CategoryMouse)
	};
}
