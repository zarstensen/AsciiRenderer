#include "arpch.h"
#include "Terminal.h"
#include "Asciir/Core/Application.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Event/TerminalEvent.h"

#ifdef AR_WIN
#include "Asciir/Platform/Windows/WinEventListener.h"
#elif defined AR_LINUX
#endif

namespace Asciir
{
	TerminalEventHandler::TerminalEventHandler()
	{
#ifdef AR_WIN
		m_event_listener = new WinEventListener();
#elif defined(AR_LINUX)
#error Events Not Implemented Yet
#endif
	}

	TerminalEventHandler::~TerminalEventHandler()
	{
		m_event_listener->stop();
	}

	void TerminalEventHandler::onStart()
	{
		// setup Terminal close callbacks
		signal(SIG_CTRL_C, onCloseSignal);
		signal(SIG_CMD_CLOSE, onCloseSignal);

		// setup event listener
		Input::setEventListener(m_event_listener);
		m_event_listener->start(m_event_callback);
	}

	void TerminalEventHandler::pollInput(const TerminalRenderer::TRUpdateInfo& terminal_udpate_info)
	{
		m_event_listener->pollState();
		Input::pollState(terminal_udpate_info);
	}

	void onCloseSignal(int signal)
	{
		// pass the terminal closed event onto the application
		TerminalClosedEvent event(signal);
		ARApp::getApplication()->getTermEvtHandler().EventCallback(event);
	}

	void TerminalEventHandler::setEventCallback(const EventCallbackFp& callback)
	{
		m_event_callback = callback;
	}

	void TerminalEventHandler::EventCallback(Event& e)
	{
		m_event_callback(e);
	}
}