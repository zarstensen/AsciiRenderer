#pragma once

#include "Asciir/Core/Core.h"
#include "Asciir/Event/Event.h"

namespace Asciir
{
	class Layer
	{
	public:
		
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAdd() {};
		virtual void onRemove() {};
		virtual void onUpdate() {};
		virtual void onEvent(Event& e) {};

		const std::string& getName() const {
			return m_debugName;
		}

	protected:
		bool m_enabled;
		std::string m_debugName;
	};
}