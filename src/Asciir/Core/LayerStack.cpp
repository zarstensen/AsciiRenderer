#include "arpch.h"
#include "LayerStack.h"

namespace Asciir
{
	LayerStack::LayerStack()
	{
		m_layerInsert = begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* target_overlayer)
	{
		auto iter = std::find(begin(), end(), target_overlayer);

		if (iter != end())
		{
			m_layers.erase(iter);
			m_layerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* target_overlay)
	{
		auto iter = std::find(begin(), end(), target_overlay);

		if (iter != end())
			m_layers.erase(iter);
	}
}