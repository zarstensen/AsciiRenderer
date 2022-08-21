#include "arpch.h"

#include "Application.h"
#include "Asciir/Input/Input.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "Asciir/Rendering/Renderer.h"

#include <ChrTrc.h>

// macro shortcut for converting an ARApp method to a event callback function
#define AR_TO_EVENT_CALLBACK(e) std::bind(&ARApp::e, this, std::placeholders::_1)

namespace Asciir
{
	ARApp::ARApp(const std::string& title, TermVert term_size)
		: m_terminal_renderer(TerminalRenderer::TerminalProps(title, term_size))
	{
		m_scene.allocate();
		m_terminal_evt.setEventCallback(AR_TO_EVENT_CALLBACK(onEvent));
	}

	ARApp* ARApp::i_app = nullptr;

	void ARApp::startMainLoop()
	{
		AR_CORE_INFO("Started application");
		
		// make sure terminal properties are avaliable when onStart is called.
		m_terminal_renderer.update();

		// notify each layer that the application has started
		for (Layer* layer : m_layerStack)
			layer->onStart();

		m_terminal_evt.onStart();

		// submits might have happened in the onStart calls, so swap the renderer queues.

		Renderer::swapQueues();
		render();

		// setup first render pass
		m_running = true;
		m_app_start = getTime();
		m_frame_count = 0;

		m_last_frame_start = m_app_start;
		
		// start the main loop
		run();
	}

	// main loop
	void ARApp::run()
	{
		AR_CORE_INFO("Running application");
		m_last_frame_start = getTime();

		while (m_running)
		{
			CT_MEASURE_N("Main Loop");

			// calculate the timeinterval from the last frame to the current frame
			DeltaTime curr_frame_start = getTime();
			DeltaTime d_time(curr_frame_start - m_last_frame_start);

			if (m_tmp_scene)
			{
				m_scene = m_tmp_scene;
				m_tmp_scene = nullptr;
			}

			{
			CT_MEASURE_N("System Processing");
			// scene systems are run *after* layer updates
			m_scene->runSystems();
			}

			{
			CT_MEASURE_N("Layer Updates");
			// update all layers on the layer stack
			for (Layer* layer : m_layerStack)
				layer->onUpdate(d_time);
			}

			m_last_frame_start = curr_frame_start;
			m_frame_count++;

			// swap the render queues
			Renderer::swapQueues();

			// render the next frame and print it to the terminal
			render();

			CT_MEASURE_N("WAIT");

			Renderer::waitMinDT(getTime() - curr_frame_start);
		}
	}

	void ARApp::render()
	{

		TerminalRenderer::TRUpdateInfo update_info;

		// the terminal should be updated before the render queue is flushed, otherwise size data will be outdated
		{
		CT_MEASURE_N("Terminal update");
		update_info = m_terminal_renderer.update();
		}

		{
		CT_MEASURE_N("Render Frame");
		// print the current queue to the terminal
		Renderer::flushRenderQueue(DeltaTime(m_last_frame_start - m_app_start), m_frame_count);
		}

		{
		CT_MEASURE_N("Print To Console");
		m_terminal_renderer.draw();
		}
		
		// as a new frame is shown, any previous inputs to the terminal should be relative to the last frame, so poll the terminal inputs
		// this is done in the render thread, as this function is dependent on info from the render call
		{
		CT_MEASURE_N("Poll Input");
		m_terminal_evt.pollInput(update_info);
		}
	}

	void ARApp::onEvent(Event& e)
	{
		// the ARApp should handle the TerminalClosedEvent
		EventHandler handler(e);
		handler.handle<TerminalClosedEvent>(AR_TO_EVENT_CALLBACK(onTerminalClose));

		for (auto iter = m_layerStack.end(); iter != m_layerStack.begin();)
		{
			(*--iter)->onEvent(e);

			if (e.handled) break;
		}
	}

	void ARApp::pushLayer(Layer* layer)
	{
		// generate appropiate events
		m_layerStack.pushLayer(layer);
		layer->onAdd();
	}

	void ARApp::popLayer(Layer* layer)
	{
		// generate appropiate events
		layer->onRemove();
		m_layerStack.popLayer(layer);
	}

	void ARApp::pushOverlay(Layer* overlay)
	{
		// generate appropiate events
		m_layerStack.pushOverlay(overlay);
		overlay->onAdd();
	}

	void ARApp::popOverlay(Layer* overlay)
	{
		// generate appropiate events
		overlay->onRemove();
		m_layerStack.popOverlay(overlay);
	}

	void ARApp::load(ARApp* app)
	{
#ifdef AR_DEBUG
		if (i_app)
			AR_CORE_ERR("Cannot create more than one ARApp");
#endif
		i_app = app;
	}

	// signal requires the function to be static,
	// so we need to have static acsess to the app for onTerminalClosed event
	ARApp* const ARApp::getApplication()
	{
		return i_app;
	}

	TerminalEventHandler& ARApp::getTermEvtHandler()
	{
		return m_terminal_evt;
	}

	TerminalRenderer& ARApp::getTermRenderer()
	{
		return m_terminal_renderer;
	}

	// stop main loop on terminal close
	bool ARApp::onTerminalClose(TerminalClosedEvent&)
	{
		AR_CORE_INFO("Stopping main loop");
		m_running = false;
		return true;
	}
}
