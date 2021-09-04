#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Event/Event.h"

namespace Asciir
{
	class Layer
	{
	public:

		Layer(const std::string& name = "Layer");
		virtual ~Layer() {};

		// gets called when the layer is pushed on the layer stack
		virtual void onAdd() {};
		// gets called when the engine is started if it is already on the layer stack
		virtual void onStart() {};
		// gets called when the layer is removed from the layer stack
		virtual void onRemove() {};
		// gets called when the engine is updating
		virtual void onUpdate(DeltaTime delta_time);
		// gets called before onUpdate with all the events polled
		virtual void onEvent(Event& e);

		const std::string& getName() const {
			return m_debugName;
		}

	protected:
		bool m_enabled;
		std::string m_debugName;
	};
}