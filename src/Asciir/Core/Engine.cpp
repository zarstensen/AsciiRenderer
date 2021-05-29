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
		m_terminal.setEventCallback(AR_BIND_EVENT_CALLBACK(onEvent));
	}

	AREngine* AREngine::i_engine = nullptr;

	AREngine::~AREngine() {}

	void AREngine::start()
	{
		AR_CORE_INFO("Started engine");
		for (Layer* layer : m_layerStack)
			layer->onStart();
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
			m_terminal.pollInput();

			for (Layer* layer: m_layerStack)
				layer->onUpdate();

			m_terminal.onUpdate();
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
	
	Terminal& AREngine::getTerminal()
	{
		return m_terminal;
	}

	// stop main loop on terminal close
	bool AREngine::onTerminalClose(TerminalClosedEvent& e)
	{
		
		AR_CORE_INFO("Stopping main loop");
		m_running = false;
		return true;
	}
}