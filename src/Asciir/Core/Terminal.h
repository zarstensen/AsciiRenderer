#pragma once

#include "Asciir/Event/Event.h"
#include "Asciir/Rendering/TerminalRenderer.h"
#include "Asciir/Input/EventListener.h"
#include "Asciir/Maths/Vertices.h"

namespace Asciir
{
	/// @brief class responsible for detecting and distributing any events related to the terminal
	class TerminalEventHandler
	{
	public:
		/// @brief 
		using EventCallbackFp = std::function<void(Event&)>;

		/// @brief constructor. Sets up the platform specifcic event listener
		TerminalEventHandler();
		/// @brief destructor. Stops the event listener
		~TerminalEventHandler();

		/// @brief initial startup function
		/// 
		/// sets up the terminal event handler.
		/// the main event handler has this called on application startup.
		void onStart();
		/// @brief polls the current inputs relative to the terminal, as well as generates specific events based on the update info from the renderer
		/// @param terminal_update_info a TRUpdateInfo return from the most recent call to TerminalRenderer::render() or TErminalRenderer::update()
		void pollInput(const TerminalRenderer::TRUpdateInfo& terminal_udpate_info);

		/// @brief passes the recieved event onto the userdefined event handler
		/// @param e the event to be passed on
		void EventCallback(Event& e);

		/// @brief sets the event callback to recieve events passed on from the event handler
		/// 
		/// all events have something to do with the terminal, some examples are:
		/// these events include, terminal resize events, terminal name change events, terminal position events, terminal input events and so on.
		/// 
		/// @param callback callback function to recieve the event 
		void setEventCallback(const EventCallbackFp& callback);

	protected:
		/// @brief refrence to the platform specific event listener
		Ref<EventListener> m_event_listener;
		EventCallbackFp m_event_callback;
	};

	/// @brief function called when the program is interuppted either by a ctrl-c interrupt, or when the user closes the terminal window
	/// @param signal the type of the interrupt
	void onCloseSignal(int signal);
}
