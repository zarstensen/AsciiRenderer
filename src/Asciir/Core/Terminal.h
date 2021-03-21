#pragma once

namespace Asciir
{
	class Terminal
	{
	public:
		Terminal();
		virtual ~Terminal();

		void start();
	};

	Terminal* CreateTerminal(std::vector<std::string> args);
}
