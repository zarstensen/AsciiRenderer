#pragma once
#include "Layer.h"

namespace Asciir
{
	/// @brief class responsible for containing layers and providing ways of modifying the stack.
	///
	/// the layerstack is split into two parts, the main layer part and the overlay part.
	/// 
	/// these two parts are essentially two seperate layerstacks, combined into one.
	/// 
	/// meaning if a layer is added to the layerstack, it will be added to the top of the layer part, and completely ignore the overlay stack.
	/// the same is true for overlays.
	/// 
	/// as the name suggests, the overlay stack is on top of the layerstack, so overlays will always be ontop of other layers.
	/// 
	/// > EXAMPLE:
	/// >
	/// >             layer part       overlay part  
	/// >             v                v  
	/// > layerstack: [l4, l3, l2, l1, ol4, ol3, ol2, ol1]  
	/// >
	/// > push l5  
	/// >             layer part         overlay part  
	/// >             v                  v   
	/// > layerstack: [l5, l4, l3, l2, l1, ol4, ol3, ol2, ol1]  
	/// >
	/// > push ol5  
	/// >             layer part         overlay part
	/// >             v                  v  
	/// > layerstack: [l5, l4, l3, l2, l1, ol5, ol4, ol3, ol2, ol1]
	/// 
	/// 
	/// should only be used through an application, as the layerstack do not generate any events.
	/// @see ARApp::pushLayer
	/// @see ARApp::popOverlay
	/// @see ARApp::pushLayer
	/// @see ARApp::popOverlay
	/// 
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		/// @brief pushes the passed layer to the top of the layer stack.
		/// @param layer layer to be pushed.
		void pushLayer(Layer* layer);
		/// @brief pushed the passed overlay to the top of the layer stack.
		/// @param overlay 
		void pushOverlay(Layer* overlay);
		/// @brief removes passed layer from the layerstack.
		/// does nothing if layer does not exist on the layerstack.
		/// @note layer is not automaticly deallocated if pop is used.
		/// @param layer layer to remove
		void popLayer(Layer* layer);
		/// @brief removes passed overlay from the layerstack.
		/// @note layer is not automaticly deallocated if pop is used.
		/// @param overlay overlay to remove.
		void popOverlay(Layer* overlay);

		/// @brief provides the beginning of an iterator that iterates over each layer in the layerstack, in order.
		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		/// @brief provides the end of an iterator that iterates over each layer in the layerstack, in order.
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:

		std::vector<Layer*> m_layers;
		/// @brief stores the location of which overlays start in the layerstack.
		std::vector<Layer*>::iterator m_overlay_start;
	};
}
