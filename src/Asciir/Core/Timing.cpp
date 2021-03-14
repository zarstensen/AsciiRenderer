#include "Timing.h"

namespace Asciir
{
	duration getTime()
	{
		return clock::now().time_since_epoch();
	}


	long long convertMilsec(const duration& dur)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	}
}
