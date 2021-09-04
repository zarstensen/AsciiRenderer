#include "arpch.h"
#include "Layer.h"

namespace Asciir
{
	Layer::Layer(const std::string& name)
		:m_debugName(name) {}

	void Layer::onUpdate(DeltaTime) {}

	void Layer::onEvent(Event&) {}
}