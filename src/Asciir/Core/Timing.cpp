#include "arpch.h"
#include "Timing.h"

namespace Asciir
{
	duration getTime()
	{
		return clock::now().time_since_epoch();
	}

	void sleep(DeltaTime dt)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(dt.nanoSeconds()));
	}

	long long castMilli(const duration& dur)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	}

	DeltaTime castRealMilli(const duration& dur)
	{
		return DeltaTime(Real(std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count()) / (Real) 1000000);
	}

	std::ostream& operator<<(std::ostream& stream, const DeltaTime& time)
	{
		stream << time.milliSeconds() << " ms";
		return stream;
	}
}