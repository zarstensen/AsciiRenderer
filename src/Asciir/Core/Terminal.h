#pragma once

#include "Asciir/Event/Event.h"

namespace Asciir
{

	struct TerminalProps
	{
		std::string Title;

		TerminalProps(const std::string& title = "Asciir Engine")
			: Title(title)
		{}
	};

	class Terminal
	{
	public:
		using EventCallback = std::function<void(Event&)>;


		Terminal(const TerminalProps& props = TerminalProps());
		~Terminal() {}

		unsigned int getWidth();
		unsigned int getHeight();



		void setEventCallback(const EventCallback& callback);

		static Terminal* create(const TerminalProps& props = TerminalProps());

	};
}
