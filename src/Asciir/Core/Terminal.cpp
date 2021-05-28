#include "arpch.h"
#include "Terminal.h"
#include "Asciir/Core/Engine.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Event/TerminalEvent.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WinEventListener.h"
#elif defined AR_LINUX
#endif

namespace Asciir
{
	Terminal::Terminal(const TerminalProps& props)
		: m_terminal_render(props.title, props.bufferSize)
	{

		#ifdef AR_WIN
		m_event_listener = std::make_shared<WinEventListener>();
		#elif defined(AR_LINUX)
		#error Events Not Implemented Yet
		#endif

		m_terminal_render.resize(props.size);
	}

	Terminal::~Terminal()
	{
		m_event_listener->stop();
	}
	
	void Terminal::onStart()
	{
		// Terminal close callbacks
		signal(SIG_CTRL_C, onCloseSignal);
		signal(SIG_CMD_CLOSE, onCloseSignal);
		Input::setEventListener(m_event_listener);
		m_event_listener->start(m_event_callback);
	}

	void Terminal::onUpdate()
	{
		m_terminal_render.draw();
	}
	
	void Terminal::pollInput()
	{
		m_event_listener->pollState();
		Input::pollState(m_terminal_render.update());
	}
	
	void onCloseSignal(int signal)
	{
		TerminalClosedEvent event(signal);
		AREngine::getEngine()->getTerminal().EventCallback(event);
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
