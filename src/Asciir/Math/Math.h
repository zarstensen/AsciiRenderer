#pragma once

#include "Asciir/Core/Core.h"
#include "vertices.h"

namespace Asciir
{

	const extern Real PI;

	Real degToRad(Real degrees);
	Real radToDeg(Real radians);

	// compare floats with a margin of error
	bool fcompare(Real a, Real b, Real margin = 0);

	Real round(Real val, Real base = 1);
	Coord round(const Coord& coord, Real base = 1);
	Real ceil(Real val, Real base = 1);
	Coord ceil(const Coord& coord, Real base = 1);
	Real floor(Real val, Real base = 1);
	Coord floor(const Coord& coord, Real base = 1);

}
