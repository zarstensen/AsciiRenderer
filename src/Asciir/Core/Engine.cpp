#include "arpch.h"
#include "Engine.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"

namespace Asciir
{
	AREngine::AREngine()
	{
		m_terminal = Terminal::create();
		m_terminal->setEventCallback(AR_BIND_EVENT_CALLBACK(onEvent));
	}

	AREngine* AREngine::i_engine = nullptr;

	AREngine::~AREngine() {}

	void AREngine::start()
	{
		AR_CORE_INFO("Started engine");
		m_terminal->onStart();
		m_running = true;
		run();
	}
	
	// main loop
	void AREngine::run()
	{
		AR_CORE_INFO("Running engine");
		while (m_running)
		{
			m_terminal->updateInput();

			// terminal event handling
			if (Input::isTerminalMoved())
			{
				auto e = Input::getTerminalMovedEvent();
				onEvent(e);
			}

			if (Input::isTerminalResized())
			{
				auto e = Input::getTerminalResizedEvent();
				onEvent(e);
			}

			// key event handling

			// loop over all key codes except UNDEFINED (0)
			for (int code = 1; code < KEY_CODE_COUNT; code++)
			{
				if (Input::isKeyToggled((Key)code) || Input::isKeyPressed((Key)code))
				{
					// if key is not toggled it must be pressed
					auto e = std::get<KeyPressedEvent>(Input::getKeyEvent((Key)code));
					onEvent(e);
				}
				else if (Input::isKeyUntoggled((Key)code))
				{
					// if key is not toggled it must be pressed
					auto e = std::get<KeyReleasedEvent>(Input::getKeyEvent((Key)code));
					onEvent(e);
				}
			}

			// mouse events
			
			// mouse key events
			
			// loop over all mouse codes except UNDEFINED (0)
			for (int code = 1; code < MOUSE_CODE_COUNT; code++)
			{
				// skip control break processing code
				if (code == 0x03) code++;

				if (Input::isMouseToggled((MouseKey)code))
				{
					// if key is not toggled it must be pressed
					auto e = std::get<MousePressedEvent>(Input::getMouseKeyEvent((MouseKey)code));
					onEvent(e);
				} 
				else if (Input::isMouseUntoggled((MouseKey)code))
				{
					// if key is not toggled it must be pressed
					auto e = std::get<MouseReleasedEvent>(Input::getMouseKeyEvent((MouseKey)code));
					onEvent(e);
				}

			}

			// mouse move events
			
			if (Input::isMouseMoved())
			{
				auto e = Input::getMouseMovedEvent();
				onEvent(e);
			}

			for (Layer* layer: m_layerStack)
				layer->onUpdate();

			m_terminal->onUpdate();
		}
	}
	
	void AREngine::onEvent(Event& e)
	{
		EventHandler handler(e);
		handler.handle<TerminalClosedEvent>(AR_BIND_EVENT_CALLBACK(onTerminalClose));


		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin();)
		{
			(*--iter)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	void AREngine::PushLayer(Layer* layer)
	{
		layer->onAdd();
		m_layerStack.PushLayer(layer);
	}

	void AREngine::create(AREngine* engine)
	{
		#ifdef AR_DEBUG
		if (i_engine)
			AR_CORE_ERR("Cannot create more than one AREngine");
		#endif
		i_engine = engine;
	}

	// signal requires the function to be static,
	// so we need to have static acsess to the engine for onTerminalClosed event
	AREngine* const AREngine::getEngine()
	{
		return i_engine;
	}
	
	Terminal* const AREngine::getTerminal()
	{
		return m_terminal.get();
	}

	// stop main loop on terminal close
	bool AREngine::onTerminalClose(TerminalClosedEvent& e)
	{
		
		AR_CORE_INFO("Stopping main loop");
		m_running = false;
		return true;
	}
}