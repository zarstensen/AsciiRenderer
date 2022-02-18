#pragma once

#include "Asciir/Core/Core.h"

namespace Asciir
{
	/// @brief namespace for literal operators in the Asciir library
	namespace AsciirLiterals
	{
		/// @brief converts the literal from degrees to radians
		Real operator "" _deg(long double degrees);
		/// @brief converts the literal from degrees to radians
		Real operator "" _deg(unsigned long long int degrees);

	}
}
