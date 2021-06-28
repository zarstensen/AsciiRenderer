#pragma once

#include "Asciir/Core/Core.h"

namespace Asciir
{
	namespace AsciirLiterals
	{
		// converts the literal from degrees to radians
		Real operator "" _deg(long double degrees);
		Real operator "" _deg(unsigned long long int degrees);
	}
}
