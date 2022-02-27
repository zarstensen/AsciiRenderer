#include "arpch.h"
#include "AsciirLiterals.h"
#include "Asciir/Maths/Maths.h"

namespace Asciir
{
	namespace AsciirLiterals
	{
		constexpr Real operator""_deg(long double degrees)
		{
			return degToRad((Real)degrees);
		}
		
		constexpr Real operator""_deg(unsigned long long int degrees)
		{
			return degToRad((Real)degrees);
		}
	}
}