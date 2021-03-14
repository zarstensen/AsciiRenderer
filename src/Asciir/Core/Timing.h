#pragma once

#include <chrono>

namespace Asciir
{
	using clock = std::chrono::steady_clock;
	using duration = std::chrono::steady_clock::duration;

	duration getTime();
	long long convertMilsec(const duration& dur);

}