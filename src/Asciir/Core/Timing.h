#pragma once

#include "Core.h"

namespace Asciir
{
	using clock = std::chrono::steady_clock;
	using duration = clock::duration;

	class DeltaTime
	{
		Real m_time_ms = 0;
	public:
		DeltaTime(Real ms = 0)
			: m_time_ms(ms) {}

		static DeltaTime FPS(Real fps) { return DeltaTime(1 / fps * 1e+3); }

		Real seconds() const { return m_time_ms / 1e+3; }
		Real milliSeconds() const { return m_time_ms; }
		long long nanoSeconds() const { return  (long long)(m_time_ms * 1e+6); }
		Real fps() const { return 1 / seconds(); }

		operator Real() const { return milliSeconds(); }
		operator duration() const { return duration(nanoSeconds()); }
	};

	duration getTime();
	void sleep(DeltaTime millsec);

	long long castMilli(const duration& dur);

	DeltaTime castRealMilli(const duration& dur);


	std::ostream& operator<<(std::ostream& stream, const DeltaTime& time);

}
