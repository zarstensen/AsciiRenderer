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

		int getSignal() { return m_signal; }

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

		TermVert getSize() const { return m_size; }
		TermVert getDiff() const { return m_size_diff; }

		bool client() { return m_client; }

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

		Coord getPos() const { return m_pos; }
		Coord getOff() const { return m_offset; }

		bool client() { return m_client; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "TerminalMoved pos: " << getPos() << " offset: " << getOff();

			return msg.str();
		}

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalMoved)
	};

	class TerminalFocusedEvent: public Event
	{
		TerminalFocusedEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalFocused)
	};

	class TerminalLostFocusEvent : public Event
	{
		TerminalLostFocusEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryTerminal)
		EVENT_TYPE_DEFINE(TerminalLostFocus)
	};
}
