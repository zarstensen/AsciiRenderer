#pragma once

#include "Vector.h"

namespace Asciir
{

	// used with lines represented in the terminal, accounts for tile size when checking for intersect
	class TermLine
	{
	public:
		TermVert a, b;

		TermLine() = default;
		TermLine(TermVert a, TermVert b);

		TInt length();

		TermVert at(TInt pos);

		TInt getX(TInt y);
		TInt getY(TInt x);

		bool intersects(TermVert point);
	};

	class Line
	{
	public:
		RealVector direction, offset;

		Line() = default;
		Line(Real a, Real b);
		Line(RealVector offset, RealVector direction);

		Real fx(Real x) const;
		Real fy(Real y) const;

		Real slope() const;
		Real a() const;
		Real yIntercept() const;
		Real b() const;
		Real xIntercept() const;

		RealVertex intersect(const Line& other);
		bool intersects(const RealVertex& point, Real margin = 0);

	};
}
