#pragma once

#include "Event.h"

namespace Asciir
{
	class TerminalCloseEvent : public Event
	{
		TerminalCloseEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalClose)
	};

	class TerminalResizeEvent : public Event
	{
	protected:
		unsigned int m_width, m_height;
	public:
		TerminalResizeEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalResize)
	};

	class TerminalFocusEvent: public Event
	{
		TerminalFocusEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalFocus)
	};

	class TerminalLostFocusEvent : public Event
	{
		TerminalLostFocusEvent() {}

		EVENT_CATEGORY_DEFINE(CategoryGame)
		EVENT_TYPE_DEFINE(TerminalLostFocus)
	};
}
