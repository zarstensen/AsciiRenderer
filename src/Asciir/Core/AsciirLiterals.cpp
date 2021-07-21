#include "arpch.h"
#include "AsciirLiterals.h"
#include "Asciir/Math/Math.h"

namespace Asciir
{
	namespace AsciirLiterals
	{
		Real operator""_deg(long double degrees)
		{
			return degToRad((Real)degrees);
		}
		Real operator""_deg(unsigned long long int degrees)
		{
			return degToRad((Real)degrees);
		}
	}
}