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
		: m_render_thread(&ARApp::render, this), m_terminal_renderer(TerminalRenderer::TerminalProps(title, term_size))
	{
		m_terminal_evt.setEventCallback(AR_TO_EVENT_CALLBACK(onEvent));
	}

	ARApp* ARApp::i_app = nullptr;

	void ARApp::startMainLoop()
	{
		AR_CORE_INFO("Started application");
		
		// notify each layer that the application has started
		for (Layer* layer : m_layerStack)
			layer->onStart();

		m_terminal_evt.onStart();

		// setup first render pass
		m_running = true;
		m_app_start = getTime();
		m_frame_count = 0;

		m_last_frame_start = m_app_start;

		m_terminal_renderer.update();

		m_render_thread.start();

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
			duration curr_frame_start = getTime();
			DeltaTime d_time(castRealMilli(curr_frame_start - m_last_frame_start));

			{
			CT_MEASURE_N("Layer Updates");
			// update all layers on the layer stack
			for (Layer* layer : m_layerStack)
				layer->onUpdate(d_time);
			}

			// wait for rendering to finish
			m_render_thread.joinLoop();
			m_last_frame_start = curr_frame_start;
			m_frame_count++;

			// swap the render queues
			Renderer::swapQueues();

			// begin rendering next frame
			m_render_thread.startLoop();

			Renderer::waitMinDT(castRealMilli(getTime() - curr_frame_start));
		}
	}

	void ARApp::render()
	{
		{
		CT_MEASURE_N("Render Frame");
		// print the current queue to the terminal
		Renderer::flushRenderQueue(DeltaTime(castRealMilli(m_last_frame_start - m_app_start)), m_frame_count);
		}

		TerminalRenderer::TRUpdateInfo update_info;

		{
		CT_MEASURE_N("Print To Console");
		update_info = m_terminal_renderer.render();
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

		m_terminal_renderer.update();
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

		m_terminal_renderer.update();
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
