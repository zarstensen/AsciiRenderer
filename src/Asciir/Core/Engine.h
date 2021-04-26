#pragma once

#include "Terminal.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "LayerStack.h"

namespace Asciir
{
	


	class AREngine
	{
	protected:
		AREngine();
	public:
		virtual ~AREngine();

		virtual void start();

		void run();

		void onEvent(Event& event);
	
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static void create(AREngine* engine);

		static AREngine* const getEngine();
		Terminal* const getTerminal();
		

	private:

		bool onTerminalClose(TerminalClosedEvent& e);

		std::unique_ptr<Terminal> m_terminal;
		bool m_running = false;
		LayerStack m_layerStack;

		static AREngine* i_engine;
	};

	AREngine* CreateEngine(std::vector<std::string> args);
}
