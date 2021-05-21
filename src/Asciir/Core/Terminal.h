#pragma once

#include "Asciir/Event/Event.h"
#include "Asciir/Rendering/TerminalRender.h"
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
		~Terminal() = default;

		static std::unique_ptr<Terminal> create(const TerminalProps& props = TerminalProps());

		void onStart();
		void onUpdate();
		void updateInput();

		TermVert getSize() const;
		Coord getPos() const;
		TerminalRender* const getRenderer();

		void EventCallback(Event& e);
		void setEventCallback(const EventCallbackFp& callback);

	protected:
		TerminalRender m_terminal_render;

		EventCallbackFp m_event_callback;

	};

	void onCloseSignal(int signal);
}
