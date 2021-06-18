#pragma once

#include "Core.h"

namespace Asciir
{
	using clock = std::chrono::steady_clock;
	using duration = std::chrono::steady_clock::duration;


	duration getTime();
	void sleep(size_t millsec);

	long long castMilli(const duration& dur);

	Real castRealMilli(const duration& dur);

	class DeltaTime
	{
		Real m_time_ms = 0;
	public:
		DeltaTime(Real ms = 0)
			: m_time_ms(ms) {}

		Real seconds() const { return m_time_ms / 1000; }
		Real milliSeconds() const { return m_time_ms; }
		Real fps() const { return 1 / seconds(); }

		operator Real() const { return milliSeconds(); }
	};

	std::ostream& operator<<(std::ostream& stream, const DeltaTime& time);

}
