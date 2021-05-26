#pragma once

#include "Asciir/Core/Core.h"

/*
*	Events are passed to onEvent functions in the engine, terminal and all of the layers.
*	They function like callbacks but are only called once per engine update
*	due to keyboard and mouse callbacks not being possible with the windows api
*	in console applications without global hooks.
*	onEvent functions are always called before update functions.
*/

namespace Asciir
{
	enum class EventType
	{
		NoneType,
		TerminalClosed, TerminalResized, TerminalMoved, TerminalFocused, TerminalLostFocus, CursorMoved,
		GameTick, GameUpdate, GameRender,
		KeyPressed, KeyReleased,
		MousePressed, MouseReleased, MouseMoved, MouseScrolled
	};
	
	typedef int EventCategory;

	constexpr EventCategory CategoryNone =				0;
	constexpr EventCategory CategoryTerminal =	BIT_SHL(0);
	constexpr EventCategory CategoryInput =		BIT_SHL(1);
	constexpr EventCategory CategoryKeyboard =	BIT_SHL(2);
	constexpr EventCategory CategoryMouse =		BIT_SHL(3);
	constexpr EventCategory CategoryCursor =	BIT_SHL(4);

	#define AR_INVALID_COORD {-1, -1}

	#define AR_EVENT_IS_VALID AR_ASSERT(m_valid, "Attempt to acsess value of non valid event.\nName: ", getName(), "\nCategory: ", getCategory(), "\nType:", (int)getStaticType())

	class Event
	{
	protected:
		bool m_valid = true;
	public:
		bool handled = false;
		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual EventCategory getCategory() const = 0;
		virtual std::string toString() const { return getName(); }
		operator std::string() const { return toString(); }
		bool isValid() { return m_valid; }

		bool inCategory(EventCategory category)
		{
			return (int)getCategory() & (int)category;
		}
	};

	// Handles an event's callbacks
	class EventHandler
	{
		template<typename T>
		using EventFp = std::function<bool(T&)>;

		Event& m_event;
	public:
		EventHandler(Event& input_event)
			: m_event(input_event) {}

		// only calls the callback if the callback type is the same as the target event.
		template<typename T>
		bool handle(EventFp<T> funcp)
		{
			if (m_event.getType() == T::getStaticType())
			{
				m_event.handled = funcp(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	};

	#define EVENT_TYPE_DEFINE(type) static EventType getStaticType() { return EventType::type; }\
										virtual EventType getType() const override { return getStaticType(); } \
										virtual const char* getName() const override { return #type; }

	#define EVENT_CATEGORY_DEFINE(category) virtual EventCategory getCategory() const override{ return category; }

	inline std::ostream& operator<<(std::ostream& stream, const Event& e)
	{
		stream << e.toString();
		return stream;
	}

	#define AR_BIND_EVENT_CALLBACK(e) std::bind(&AREngine::e, this, std::placeholders::_1)

}
