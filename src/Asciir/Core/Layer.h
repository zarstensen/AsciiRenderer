#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Event/Event.h"

namespace Asciir
{
	/// @brief base class for user defined layers
	/// 
	/// a single layer is responsible for controlling its own entities and rendering them to the terminal.
	/// every layer recieves events based on their position in the application layer stack, meaning the top layers will recieve the events first.
	/// the same logic applies to updating and rendering layers, meaning layers get updated from top to bottom, and rendered from top to bottom.
	/// 
	/// user overloadable functions:
	/// @see onAdd
	/// @see onStart
	/// @see onRemove
	/// @see onUpdate
	/// @see onEvent
	/// 
	/// the layer should override at least one of these functions, otherwise it has no functionality
	/// 
	/// furthermore, a layer can be disabled.
	/// in a disabled state a layer will act as if it has been removed from the layerstack, meaning it will not recieve any events, be updated or rendered.
	/// when enabled, it maintains its positioning as if it was never removed.
	/// 
	class Layer
	{
	public:

		/// @brief constructor for the layer class
		/// @param name the name outputted when information about this layer is logged
		Layer(const std::string& name = "Layer");
		/// @brief user overridable destructor
		virtual ~Layer() {};

		/// @brief gets called when the layer is pushed on the layer stack
		virtual void onAdd() {};
		/// @brief gets called right after the application is started, if it got added in the ARApp::start() method
		virtual void onStart() {};
		/// @brief gets called when the layer is removed from the layer stack
		virtual void onRemove() {};
		/// @brief gets called when the application is updating
		/// @param delta_time delta time between the last rendered frame and the current one
		virtual void onUpdate(DeltaTime delta_time);
		/// @brief gets called before onUpdate with all the events polled
		/// @param e the event to be handled
		virtual void onEvent(Event& e);

		// TODO: should keep?
		const std::string& getName() const {
			return m_debugName;
		}

	protected:
		bool m_enabled;
		std::string m_debugName;
	};
}