#include "arpch.h"
#include "Terminal.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Event/TerminalEvent.h"

namespace Asciir
{
	Terminal::Terminal(const TerminalProps& props)
		: m_terminal_render(props.title, props.bufferSize)
	{
		m_terminal_render.resize(props.size);
	}

	std::unique_ptr<Terminal> Terminal::create(const TerminalProps& props)
	{
		return std::make_unique<Terminal>(props);
	}
	
	void Terminal::onStart()
	{
		// Terminal close callbacks
		signal(SIG_CTRL_C, onCloseSignal);
		signal(SIG_CMD_CLOSE, onCloseSignal);
	}

	void Terminal::onUpdate()
	{
		m_terminal_render.draw();
	}
	
	void Terminal::updateInput()
	{
		Input::update(m_terminal_render.update());
	}
	
	void onCloseSignal(int signal)
	{
		TerminalClosedEvent event(signal);
		AREngine::getEngine()->getTerminal()->EventCallback(event);
	}
	
	TerminalRender* const Terminal::getRenderer()
	{
		return &m_terminal_render;
	}

	TermVert Terminal::getSize() const
	{
		return m_terminal_render.termSize();
	}

	Coord Terminal::getPos() const
	{
		return m_terminal_render.pos();
	}

	void Terminal::setEventCallback(const EventCallbackFp& callback)
	{
		m_event_callback = callback;
	}

	void Terminal::EventCallback(Event& e)
	{
		m_event_callback(e);
	}

}
