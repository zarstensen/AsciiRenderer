#pragma once

namespace Asciir
{
	class AREngine
	{
	public:
		AREngine();
		virtual ~AREngine();

		virtual void start();
	};

	AREngine* CreateEngine(std::vector<std::string> args);
}
