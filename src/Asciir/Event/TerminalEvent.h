#pragma once

#include "Event.h"

// Events related to the terminal.

namespace Asciir
{
	class TerminalClosedEvent : public Event
	{
	protected:
		int m_signal;
	public:
		TerminalClosedEvent(int signal) : m_signal(signal) {}
		TerminalClosedEvent() : m_signal(-1) { m_valid = false; }

		int getSignal() { AR_EVENT_IS_VALID; return m_signal; }

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalClosed)
	};

	class TerminalResizedEvent : public Event
	{
	protected:
		TermVert m_size;
		TermVert m_size_diff;
		bool m_client;
	public:
		TerminalResizedEvent(TermVert size, TermVert size_diff, bool by_client)
			: m_size(size), m_size_diff(size_diff), m_client(by_client)
		{}
		
		TerminalResizedEvent()
			: m_size(AR_INVALID_COORD), m_size_diff(AR_INVALID_COORD), m_client(false) { m_valid = false; }

		TermVert getSize() const { AR_EVENT_IS_VALID; return m_size; }
		TermVert getDiff() const { AR_EVENT_IS_VALID; return m_size_diff; }

		bool client() { AR_EVENT_IS_VALID; return m_client; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "TerminalResized: " << getSize();

			return msg.str();
		}

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalResized)
	};

	class TerminalMovedEvent : public Event
	{
	protected:
		Coord m_pos;
		Coord m_offset;
		bool m_client;
	public:
		TerminalMovedEvent(Coord pos, Coord offset, bool by_client)
			:m_pos(pos), m_offset(offset), m_client(by_client)
		{}

		TerminalMovedEvent()
			: m_pos(AR_INVALID_COORD), m_offset(AR_INVALID_COORD), m_client(false) { m_valid = false; }

		Coord getPos() const { AR_EVENT_IS_VALID; return m_pos; }
		Coord getOff() const { AR_EVENT_IS_VALID; return m_offset; }

		bool client() { AR_EVENT_IS_VALID; return m_client; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "TerminalMoved pos: " << getPos() << " offset: " << getOff();

			return msg.str();
		}

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalMoved)
	};

	class TerminalFocusEvent: public Event
	{
	protected:
		bool m_focused;
	public:
		TerminalFocusEvent(bool focused)
			: m_focused(focused) {}

		TerminalFocusEvent()
			: m_focused(false) { m_valid = false; }

		bool isFocused() { AR_EVENT_IS_VALID; return m_focused; }

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalFocused)
	};
}
