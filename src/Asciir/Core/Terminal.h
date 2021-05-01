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


		TerminalProps(const std::string& title = "Asciir Engine", TermVert size = { 100, 50 }, size_t buffer_size = 1024ULL * 1024ULL * 4ULL)
			: title(title), size(size), bufferSize(buffer_size)
		{}
	};

	class Terminal
	{
	public:
		using EventCallbackFp = std::function<void(Event&)>;


		Terminal(const TerminalProps& props = TerminalProps());
		virtual ~Terminal() = default;

		static std::unique_ptr<Terminal> create(const TerminalProps& props = TerminalProps());

		virtual void onStart() = 0;
		virtual void onUpdate() = 0;
		virtual void updateInput() = 0;

		TermVert getSize() const;
		Coord getPos() const;
		TerminalRender* const getRenderer();

		virtual void setEventCallback(const EventCallbackFp& callback) = 0;

	protected:
		TerminalRender m_terminal_render;

		EventCallbackFp m_event_callback;

	};
}
