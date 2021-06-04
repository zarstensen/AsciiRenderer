#pragma once

#include "Vector.h"

namespace Asciir
{
	class Line
	{
	public:
		Vector<Real> direction, offset;

		Line() = default;
		Line(Real a, Real b);
		Line(Vector<Real> direction, Vector<Real> offset = Vector<Real>());

		// direction goes from a to b
		static Line fromPoints(arVertex2D<Real> a, arVertex2D<Real> b);

		Real fx(Real x) const;
		Real fy(Real y) const;

		arVertex2D<Real> pointFromGrid(long long indx, Real resolution);

		Real slope() const;
		Real a() const;
		Real yIntercept() const;
		Real b() const;
		Real xIntercept() const;


		bool visible(arVertex2D<Real> point) const;
		static bool visibleByAll(const std::vector<Line>& lines, arVertex2D<Real> point);
		
		bool notVisible(arVertex2D<Real> point) const;
		static bool notVisibleByAll(const std::vector<Line>& lines, arVertex2D<Real> point);

		arVertex2D<Real> intersect(const Line& other);
		bool intersects(arVertex2D<Real> point, Real margin = 0);
		bool intDirection(arVertex2D<Real> point, Real margin = 0);
	};
}
