#pragma once

#include "Asciir/Core/Core.h"

namespace Asciir
{
	/// @brief namespace for literal operators in the Asciir library
	namespace AsciirLiterals
	{
		/// @brief converts the literal from degrees to radians
		constexpr Real operator "" _deg(long double degrees);
		/// @brief converts the literal from degrees to radians
		constexpr Real operator "" _deg(unsigned long long int degrees);

		/// @brief makes sure the literal has the Real datatype.  
		/// use this to avoid truncation warnings if using half precission Real's.
		constexpr Real operator "" _R(long double real) { return (Real)real; }
		/// @brief @see operator "" _R(long double)
		constexpr Real operator "" _R(unsigned long long int real) { return (Real)real; }

	}
}
