#include "Math.h"

namespace Asciir
{
	const Real PI = (Real)M_PI;

	Real degToRad(Real degrees)
	{
		return degrees * PI / (Real) 180;
	}

	Real radToDeg(Real radians)
	{
		return radians * (Real)180 / PI;
	}
	
	bool fcompare(Real a, Real b, Real margin)
	{
		return  a < b + margin / 2 && a > b - margin / 2;
	}

	Real round(Real val, Real base)
	{
		Real mod = fmod(val, base);
		return val - mod + (mod < base / 2 ? 0 : base);
	}

	Real ceil(Real val, Real base)
	{
		return val - fmod(val, base) + base;
	}

	Real floor(Real val, Real base)
	{
		return val - fmod(val, base);
	}
}
