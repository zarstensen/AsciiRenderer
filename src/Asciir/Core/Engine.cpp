#include "arpch.h"
#include "Engine.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Rendering/Renderer.h"

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

		m_terminal.onStart();

		m_running = true;
		m_engine_start = getTime();
		m_frame_count = 0;

		m_last_frame_start = m_engine_start;

		run();
	}

	// main loop
	void AREngine::run()
	{
		AR_CORE_INFO("Running engine");
		m_last_frame_start = getTime();
		while (m_running)
		{
			duration curr_frame_start = getTime();
			DeltaTime d_time(castRealMilli(curr_frame_start - m_last_frame_start));
			m_last_frame_start = curr_frame_start;
			m_frame_count++;

			for (Layer* layer : m_layerStack)
				layer->onUpdate(d_time);

			if (m_render_thread.joinable())
				m_render_thread.join();

			Renderer::swapQueues();

			m_render_thread = std::thread(&AREngine::render, this);

			Renderer::waitMinDT(castRealMilli(getTime() - curr_frame_start));
		}
	}

	void AREngine::render()
	{
		Renderer::flushRenderQueue(DeltaTime(castRealMilli(m_last_frame_start - m_engine_start)), m_frame_count);
		m_terminal.onUpdate();
		m_terminal.pollInput();
	}

	void AREngine::onEvent(Event& e)
	{
		EventHandler handler(e);
		handler.handle<TerminalClosedEvent>(AR_BIND_EVENT_CALLBACK(onTerminalClose));

		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin();)
		{
			(*--iter)->onEvent(e);

			if (e.handled) break;
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