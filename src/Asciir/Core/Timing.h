#pragma once

#include "Core.h"

namespace Asciir
{
	using clock = std::chrono::steady_clock;
	using duration = clock::duration;

	/// @brief data class for storing time intervals with minimal overhead
	class DeltaTime
	{
		/// @brief stored as Real to reduce overhead at the cost of precission
		Real m_time_ms = 0;
	public:

		/// @brief constructs a DeltaTime object, from an input time
		/// @param ms time input in milliseconds
		DeltaTime(Real ms = 0)
			: m_time_ms(ms) {}

		DeltaTime(const duration& duration)
			: m_time_ms(((Real)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / (Real) 1e+6) {}

		static DeltaTime freq(Real dt) { return DeltaTime(dt).freq(); }
		static DeltaTime fps(Real dt) { return freq(dt); }

		/// @brief get the time interval in seconds
		Real seconds() const { return m_time_ms / (Real)1e+3; }
		/// @brief get the time interval in milliseconds
		Real milliSeconds() const { return m_time_ms; }
		/// @brief get the time interval in nanoseconds
		long long nanoSeconds() const { return  (long long)(m_time_ms * (Real)1e+6); }
		/// @brief converts the interval in to a frequency
		Real freq() const { return 1 / seconds(); }
		/// @brief same as freq but assumes delta time store interval inbetween two frames
		Real fps() const { return freq(); }

		/// @brief cast operator for the Real datatype, final value is in seconds
		operator Real() const { return seconds(); }
		/// @brief cast operator for the std::chrono::duration datatype, final value is in nanoseconds
		operator duration() const { return duration(nanoSeconds()); }
	};

	/// @brief get the current time
	duration getTime();
	void sleep(DeltaTime millsec);

	/// @brief convert duration to milliseconds
	long long castMilli(const duration& dur);

	/// @brief convert duration to milliseconds, whilst keeping the fractional part
	DeltaTime castRealMilli(const duration& dur);

	/// @brief let cout print DeltaTime types
	/// prints the value in milliseconds
	std::ostream& operator<<(std::ostream& stream, const DeltaTime& time);
}
