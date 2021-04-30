#include "arpch.h"
#include "WindowsTerminal.h"
#include "Log.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Core/Engine.h"

#ifdef AR_WIN

namespace Asciir
{

	int charToKey(char c)
	{
		if (c >= 'a' && c <= 'z')
			c -= 32;
		if (!(c >= '0' && c <= 'Z' || c >= '0' && c <= '9'))
		{
			std::string msg = " is not a valid key";
			AR_ERR(c, msg);
		}

		return c - 24;
	}

	WindowsTerminal::WindowsTerminal(const TerminalProps& props)
		: Terminal(props) {}

	WindowsTerminal::~WindowsTerminal() {}

	void WindowsTerminal::onStart()
	{
		// Terminal close callbacks
		signal(SIG_CTRL_C, onCloseSignal);
		signal(SIG_CMD_CLOSE, onCloseSignal);
	}

	void WindowsTerminal::onUpdate()
	{
		m_terminal_render.draw();
	}

	void WindowsTerminal::updateInput()
	{
		Input::update(m_terminal_render.update());
	}

	void WindowsTerminal::setEventCallback(const EventCallbackFp& callback)
	{
		m_event_callback = callback;
	}

	void WindowsTerminal::EventCallback(Event& e)
	{
		m_event_callback(e);
	}
	
	void onCloseSignal(int signal)
	{
		TerminalClosedEvent event(signal);
		WindowsTerminal* terminal = (WindowsTerminal*)AREngine::getEngine()->getTerminal();
		terminal->EventCallback(event);
	}
}
#endif

