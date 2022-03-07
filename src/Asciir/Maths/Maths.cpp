#include "Maths.h"

namespace Asciir
{
	const Real PI = (Real)M_PI;

	bool compareMargin(Real a, Real b, Real margin)
	{
		return  a < b + margin / 2 && a > b - margin / 2;
	}

	bool fequal(Real a, Real b, Real e)
	{
		Real delta = std::abs(a - b);

		// the epsilon value should be scaled depending on the maximum floating point value, as this one will be the most unprecise.
		Real max = std::max(std::abs(a), std::abs(b));

		return delta <= max * e;
	}

	Real round(Real val, Real base)
	{
		if (fequal(base, 1))
			return std::round(val);

		Real mod = fmod(val, base);
		return val - mod + (mod < base / 2 ? 0 : base);
	}

	Coord round(const Coord& coord, Real base)
	{
		return { round(coord.x, base), round(coord.y, base) };
	}

	Real ceil(Real val, Real base)
	{
		if (fequal(base, 1))
			return std::ceil(val);

		Real frac = fmod(val, base);

		Real result = val - frac + (fequal(frac, 0.0) ? 0 : base);

		return result;
	}

	Coord ceil(const Coord& coord, Real base)
	{
		return { ceil(coord.x, base), ceil(coord.y, base) };
	}

	Real floor(Real val, Real base)
	{
		if (fequal(base, 1))
			return std::floor(val);

		return val - fmod(val, base);
	}

	Coord floor(const Coord& coord, Real base)
	{
		return { floor(coord.x, base), floor(coord.y, base) };
	}
}