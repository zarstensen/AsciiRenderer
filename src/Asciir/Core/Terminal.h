#pragma once

#include "Asciir/Event/Event.h"
#include "Asciir/Rendering/TerminalRenderer.h"
#include "Asciir/Input/EventListener.h"
#include "Asciir/Math/Vertices.h"


namespace Asciir
{

	struct TerminalProps
	{
		std::string title;
		TermVert size;
		size_t bufferSize;


		TerminalProps(const std::string& title = "Asciir Engine", TermVert size = { 0, 0 }, size_t buffer_size = 1024ULL * 512ULL)
			: title(title), size(size), bufferSize(buffer_size)
		{}
	};

	class Terminal
	{
	public:
		using EventCallbackFp = std::function<void(Event&)>;


		Terminal(const TerminalProps& props = TerminalProps());
		~Terminal();

		void onStart();
		void onUpdate();
		void pollInput();

		TermVert getSize() const;
		Coord getPos() const;
		TerminalRenderer* const getRenderer();

		void EventCallback(Event& e);
		void setEventCallback(const EventCallbackFp& callback);

	protected:
		TerminalRenderer m_terminal_render;
		std::shared_ptr<EventListener> m_event_listener;
		EventCallbackFp m_event_callback;
	};

	void onCloseSignal(int signal);
}
