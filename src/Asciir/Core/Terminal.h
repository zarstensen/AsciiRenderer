#pragma once
#include "Core.h"
#include <vector>
#include <string>

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
