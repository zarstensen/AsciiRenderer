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

	Coord round(const Coord& coord, Real base)
	{
		return { round(coord.x, base), round(coord.y, base) };
	}

	Real ceil(Real val, Real base)
	{
		return val - fmod(val, base) + base;
	}

	Coord ceil(const Coord& coord, Real base)
	{
		return { ceil(coord.x, base), ceil(coord.y, base) };
	}

	Real floor(Real val, Real base)
	{
		return val - fmod(val, base);
	}

	Coord floor(const Coord& coord, Real base)
	{
		return { floor(coord.x, base), floor(coord.y, base) };
	}
}
