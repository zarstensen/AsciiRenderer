#include "Timing.h"
#include <thread>

namespace Asciir
{
	duration getTime()
	{
		return clock::now().time_since_epoch();
	}

	void Asciir::sleep(size_t millsec)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millsec));
	}

	long long convertMilsec(const duration& dur)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	}
}