#pragma once

#include "Asciir/Core/Core.h"

namespace Asciir
{
	enum class EventType
	{
		None,
		TerminalClose, TerminalResize, TerminalFocus, TerminalLostFocus, CursorMoved,
		GameTick, GameUpdate, GameRender,
		KeyPressed, KeyReleased,
		MousePressed, MouseReleased, MouseMoved, MouseScrolled
	};
	
	typedef int EventCategory;

	EventCategory CategoryNone =	0;
	EventCategory CategoryGame =	BIT_SHL(0);
	EventCategory CategoryInput =	BIT_SHL(1);
	EventCategory CategoryKeyboard =BIT_SHL(2);
	EventCategory CategoryMouse =	BIT_SHL(3);
	EventCategory CategoryCursor =	BIT_SHL(4);



	class Event
	{
	protected:
		bool m_handled = false;
	public:
		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual EventCategory getCategory() const = 0;
		virtual std::string toString() const { return getName(); }
		operator std::string() const { return toString(); }

		bool inCategory(EventCategory category)
		{
			return (int)getCategory() & (int)category;
		}
	};

	class EventHandler
	{
		template<typename T>
		using EventFp = std::function<bool(T&)>;

		Event& m_event;
	public:
		EventHandler(Event& input_event)
			: m_event(input_event)
		{}

		template<typename T, std::enable_if_t<std::is_same_v<T, Event>, bool> = 0>
		bool handle(EventFp<T> funcp)
		{
			if (m_event.getType() == T::getStaticType())
			{
				m_event.m_handled = funcp(*(T*)&m_event);
				return true;
			}
			return false;
		}
	};

	#define EVENT_TYPE_DEFINE(type) static EventType getStaticType() { return EventType::##type; }\
										virtual EventType getType() const override { return getStaticType(); } \
										virtual const char* getName() const override { return #type; }

	#define EVENT_CATEGORY_DEFINE(category) virtual EventCategory getCategory() const override{ return category; }

	std::ostream& operator<<(std::ostream& stream, const Event& e)
	{
		stream << e.toString();
		return stream;
	}

}
