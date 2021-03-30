#pragma once

#include "Event.h"

namespace Asciir
{
	class TerminalClosedEvent : public Event
	{
		TerminalClosedEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalClosed)
	};

	class TerminalResizedEvent : public Event
	{
	protected:
		unsigned int m_width, m_height;
	public:
		TerminalResizedEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{}

		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }

		virtual std::string toString() const override
		{
			std::stringstream msg;

			msg << "TerminalResized: " << getWidth() << ',' << getHeight();

			return msg.str();
		}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalResized)
	};

	class TerminalFocusedEvent: public Event
	{
		TerminalFocusedEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalFocused)
	};

	class TerminalLostFocusEvent : public Event
	{
		TerminalLostFocusEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalLostFocus)
	};
}
