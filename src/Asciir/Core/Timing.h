#pragma once

#include "Core.h"

namespace Asciir
{
	using clock = std::chrono::steady_clock;
	using duration = std::chrono::nanoseconds;

	/// @brief data class for storing time intervals with minimal overhead
	class DeltaTime
	{
	protected:

		duration m_time;
	
	public:

		/// @brief constructs a DeltaTime object, from an input time
		/// @param ms time input in milliseconds
		DeltaTime(Real s = 0)
			: m_time(std::chrono::nanoseconds((size_t)(s * (Real)1e+9))) {}

		DeltaTime(const duration& duration)
			: m_time(duration) {}

		static DeltaTime freq(Real dt) { return DeltaTime(dt).freq(); }
		static DeltaTime fps(Real dt) { return freq(dt); }

		/// @brief get the time interval in seconds
		Real seconds() const { return m_time.count() / (Real)1e+9; }
		/// @brief get the time interval in milliseconds
		Real milliSeconds() const { return m_time.count() / (Real)1e+6; }
		/// @brief get the time interval in microseconds
		Real microSeconds() const { return m_time.count() / (Real)1e+3; }
		/// @brief get the time interval in nanoseconds
		long long nanoSeconds() const { return  m_time.count(); }
		/// @brief converts the interval in to a frequency
		Real freq() const { return 1 / seconds(); }
		/// @brief same as freq but assumes delta time store interval inbetween two frames
		Real fps() const { return freq(); }

		/// @brief retrieve the underlying chrono duration variable containing the time.
		/// @return 
		duration durr() { return m_time; }

		/// @brief cast operator for the underlying chrono duratino datatype
		explicit operator duration() { return m_time; }
		/// @brief cast operator for the Real datatype, final value is in seconds
		operator Real() const { return seconds(); }
		/// @brief cast operator for integer data types, final value is in nano seconds
		explicit operator long long() const { return nanoSeconds(); }

		DeltaTime& operator+=(const DeltaTime& other) { m_time += other.m_time; return *this; }
		DeltaTime& operator-=(const DeltaTime& other) { m_time -= other.m_time; return *this; }

		DeltaTime operator+(const DeltaTime& other) { return DeltaTime(*this) += other; }
		DeltaTime operator-(const DeltaTime& other) { return DeltaTime(*this) -= other; }

		bool operator < (const DeltaTime& other) { return m_time < other.m_time; }
		bool operator <=(const DeltaTime& other) { return m_time <= other.m_time; }
		bool operator > (const DeltaTime& other) { return m_time > other.m_time; }
		bool operator >=(const DeltaTime& other) { return m_time >= other.m_time; }
		bool operator ==(const DeltaTime& other) { return m_time == other.m_time; }


	};

	/// @brief get the current time
	DeltaTime getTime();
	void sleep(DeltaTime millsec);

	/// @brief convert duration to milliseconds
	long long castMilli(const duration& dur);

	/// @brief convert duration to milliseconds, whilst keeping the fractional part
	DeltaTime castRealMilli(const duration& dur);

	/// @brief let cout print DeltaTime types
	/// prints the value in milliseconds
	std::ostream& operator<<(std::ostream& stream, const DeltaTime& time);
}
