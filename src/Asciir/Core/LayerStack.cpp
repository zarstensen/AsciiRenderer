#include "arpch.h"
#include "LayerStack.h"

namespace Asciir
{
	LayerStack::LayerStack()
	{
		m_overlay_start = begin();
	}

	LayerStack::~LayerStack()
	{
		// automaticly deallocate any layers passed to the layerstack
		for (Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		// layer should be placed right before the overlays
		m_overlay_start = m_layers.emplace(m_overlay_start, layer) + 1;
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.push_back(overlay);
	}

	void LayerStack::popLayer(Layer* target_overlayer)
	{
		auto iter = std::find(begin(), end(), target_overlayer);

		if (iter != end())
		{
			m_layers.erase(iter);
			// the position of the overlays will shrink by one, as a layer has been removed
			m_overlay_start--;
		}
	}

	void LayerStack::popOverlay(Layer* target_overlay)
	{
		auto iter = std::find(begin(), end(), target_overlay);

		if (iter != end())
			m_layers.erase(iter);
	}
}