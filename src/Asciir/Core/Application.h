#pragma once

#include "Terminal.h"
#include "Asciir/Event/KeyEvent.h"
#include "Asciir/Event/MouseEvent.h"
#include "Asciir/Event/TerminalEvent.h"
#include "LayerStack.h"

#include <ETH.h>

namespace Asciir
{
	/// @brief base class for the user defined application class
	///
	/// this class has the responsibility of setting up the game, as well as handling events and running the main loop.
	/// the class should be inherited from a user defined class, that actually implements an application.
	/// 
	/// the user defined class should always overload the start function and the user defined part of the class should only be responsible for setting up the game itself.
	/// 
	/// 
	class ARApp
	{
	public:
		/// @brief initialize application with the passed arguments
		/// @param title the title of the console
		/// @param term_size the size of the console in characters
		ARApp(const std::string& title = "Commandline Application", TermVert term_size = { 0, 0 });
		virtual ~ARApp() {}

		/// @brief entry point for the user defined application
		/// 
		/// @attention should always push at least one layer onto the layerstack; see PushLayer(Layer*), otherwise the applikation will have no functionality,
		/// as the layers are responsible for the game logic and rendering of the game.
		/// @param args command line arguments passed from the main function
		virtual void start(const std::vector<std::string>& args) = 0;
		
		void startMainLoop();

		/// @brief the main loop of the application
		///
		/// gets called when the application should start its main loop.
		/// 
		/// inside the main loop, every layer gets cycled through and is made sure gets updated.
		/// after each main loop cycle, the rendering will begin.
		/// after the render thread has been setup, the main loop will once again cycle through the layers and update them, whilst the renderer is rendering the previous information to the console.
		/// this means the gamelogic can be processed whilst the last game frame is getting printed to the window, thus minimizing the wait time for this slow step.
		/// 
		/// > Main Loop:  
		/// > 1  Layers get updated  
		/// > 2  Rendering begins  
		/// > 3  Layers get updated whilst rendering  
		/// > 4  wait for rendering to finish  
		/// > 5  repeat from 2  
		/// 
		void run();

		/// @brief process responsible for printing the render queue onto the terminal as well as updating the terminal
		/// 
		/// as this is responsible for rendering, this function is called on a different thread from the main loop
		void render();

		/// @brief callback for all events recieved by the applications
		/// 
		/// automaticly distribute the event to all the layers in the layer stack.
		/// 
		/// @param event event to handle
		void onEvent(Event& event);

		/// @brief pushes a layer onto the layer stack
		/// 
		/// the order of the layers in the layer stack is determined by the order of PushLayer() calls
		/// @param layer layer to be pushed
		void pushLayer(Layer* layer);
		/// @brief pops the layer from the layerstack
		/// @param layer layer to be popped
		void popLayer(Layer* layer);
		/// @brief pushes a overlay onto the layer stack
		/// 
		/// an overlay will always be ontop of all other layers, otherwise they function the same.
		/// @param overlay overlay to be pushed
		void pushOverlay(Layer* overlay);
		/// @brief pops the overlay from the layerstack
		/// @param overlay overlay to be popped
		void popOverlay(Layer* overlay);
		
		/// @brief sets the passed app as the main asciir application
		/// @param app app to be loaded
		static void load(ARApp* app);

		/// @brief get the main asciir application
		static ARApp* const getApplication();

		/// @brief get the terminal event handler tied to the main application
		TerminalEventHandler& getTermEvtHandler();

		/// @brief get the terminal renderer tied to the main application
		TerminalRenderer& getTermRenderer();

	private:

		/// @brief event callback for TerminalClosedEvent
		/// stops the main loop
		/// @return true if exit success
		bool onTerminalClose(TerminalClosedEvent& e);

		/// @brief used to handle the terminal
		/// used to handle rendering to the terminal and event handling for terminal specific events
		TerminalEventHandler m_terminal_evt;
		TerminalRenderer m_terminal_renderer;
		bool m_running = false;
		LayerStack m_layerStack;
		/// @brief used to calculate the delta time between frames
		duration m_last_frame_start;
		/// @brief stores the start of the application
		duration m_app_start;
		/// @brief stores the amount of frames rendered to the terminal
		size_t m_frame_count = 0;

		/// @brief the seperate thread from which the terminal is rendered from
		/// thread gets started every time an update is finished, unless a thread already is running
		ETH::LThread m_render_thread;

		/// @brief stores the currently active application
		static ARApp* i_app;
	};
}
