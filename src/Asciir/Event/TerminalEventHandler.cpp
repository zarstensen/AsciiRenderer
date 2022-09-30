#include "arpch.h"

#include "Asciir/Event/TerminalEventHandler.h"

#include "Asciir/Core/Application.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Event/TerminalEvent.h"

#include "Asciir/Input/EventListener.h"

#include <ChrTrc.h>

namespace Asciir
{
	TerminalEventHandler::TerminalEventHandler() {}

	TerminalEventHandler::~TerminalEventHandler()
	{
		m_event_listener.stop();
	}

	void TerminalEventHandler::onStart()
	{
		// setup Terminal close callbacks
		signal(SIG_CTRL_C, onCloseSignal);
		signal(SIG_CMD_CLOSE, onCloseSignal);

		// setup event listener
		Input::setEventListener(m_event_listener);
		m_event_listener.start(m_event_callback);
	}

	void TerminalEventHandler::pollInput(const TerminalRenderer::TRUpdateInfo& terminal_udpate_info)
	{
		{
			CT_MEASURE_N("EVTL POLL");
			m_event_listener.pollState();
		}

		{
			CT_MEASURE_N("INPUT POLL");
			Input::pollState(terminal_udpate_info);
		}
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