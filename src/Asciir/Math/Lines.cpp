#include "pch/arpch.h"
#include "Lines.h"
#include "Vector.h"

namespace Asciir
{
	TermLine::TermLine(TermVert a, TermVert b)
		: a(a), b(b) {}

	TInt TermLine::length()
	{
		TermVert dvert = b - a;

		// return height if height is greater than width, otherwise return width
		if (std::abs(dvert.x) >= std::abs(dvert.y))
			return std::abs(dvert.x);	
		else
			return std::abs(dvert.y);
	}

	TermVert TermLine::at(TInt pos)
	{
		TermVert dvert = b - a;

		// slope inbetween 1 and -1
		if (std::abs(dvert.x) >= std::abs(dvert.y))
		{
			float slope = float(dvert.y) / float(dvert.x);

			// flip points if a is to the left of b
			if (dvert.x > 0)
				return { TInt(a.x + pos), TInt(a.y + std::round(((float)pos) * slope)) };
			else
				return { TInt(b.x + pos), TInt(b.y + std::round(((float)pos) * slope)) };
		}
		else
		{
			float slope = float(dvert.x) / float(dvert.y);

			// flip points if a is under b
			if (dvert.y > 0)
				return { TInt( a.x + std::round(float(pos) * slope)),	TInt(a.y + pos) };
			else
				return { TInt(b.x + std::round(((float)pos) * slope)),	TInt(b.y + pos) };
		}
	}

	TInt TermLine::getX(TInt y)
	{
		TermVert dvert = b - a;

		float slope = float(dvert.x) / float(dvert.y);
		return TInt(slope * y + a.x);
	}

	TInt TermLine::getY(TInt x)
	{
		TermVert dvert = b - a;

		float slope = float(dvert.y) / float(dvert.x);
		return TInt(slope * x + a.y);
	}

	bool TermLine::intersects(TermVert point)
	{
		TermVert vec_a = { (TInt)std::abs(b.x - a.x),		(TInt)std::abs(b.y - a.y) };
		TermVert vec_b = { a.x < point.x ? (TInt)std::abs(b.x - point.x) : (TInt)std::abs(a.x - point.x), a.x < point.x ? (TInt)std::abs(b.y - point.y) : (TInt)std::abs(a.y - point.y) };

		// return false if point is not inside line bounding box
		if (vec_a.x > vec_b.x && vec_b.x < 0 || vec_a.y > vec_b.y && vec_b.y < 0)
			return false;
		
		// return true if vec_b is a null vector
		if (vec_b.x == 0 && vec_b.y == 0)
			return true;

		if (vec_a.y == 0 && vec_b.y == 0)
			return true;
		else if (vec_a.y == 0 || vec_b.y == 0)
			return false;

		if (vec_a.x == 0 && vec_b.x == 0)
			return true;
		else if (vec_a.x == 0 || vec_b.x == 0)
			return false;

		// check if the two vectors are parallel

		double a_coeff =  double(vec_a.y) / vec_a.x;

		double b_coeff = double(vec_b.y) / vec_b.x;
		double next_b_coeff = double(vec_b.y) / (vec_b.x + 1);

		return next_b_coeff < a_coeff &&  b_coeff >= a_coeff;
	}
	
	 
	Line::Line(Real a, Real b)
		: direction(1, a), offset(0, b) {}
	
	 
	Line::Line(Vector<Real> direction, Vector<Real> offset)
		: direction(direction), offset(offset) {}
	
	 
	Line Line::fromPoints(arVertex2D<Real> a, arVertex2D<Real> b)
	{
		return Line(b - a, a);
	}
	
	 
	Real Line::fx(Real x) const
	{
		AR_ASSERT_MSG(direction.x != 0, "Cannot get function of x on a vertical line");

		return (x - offset.x) / direction.x * direction.y + offset.y;
	}
	
	 
	Real Line::fy(Real y) const
	{
		AR_ASSERT_MSG(direction.y != 0, "Cannot get function of y on a horizontal line");

		return (y - offset.y) / direction.y * direction.x + offset.x;
	}

	// divides line into a grid of a specified resolution.
	// Realhe index is the coords of the grid n times away from the center, intersecting the line
	 
	arVertex2D<Real> Line::pointFromGrid(long long indx, Real resolution)
	{
		return (direction / std::max(std::abs(direction.x), std::abs(direction.y))) / resolution * indx + offset;
	}
	
	 
	Real Line::slope() const
	{
		AR_ASSERT_MSG(direction.x != 0, "Cannot get slope of vertical line");

		return direction.y / direction.x;
	}
	
	 
	Real Line::a() const
	{
		return slope();
	}
	
	 
	Real Line::yIntercept() const
	{
		return fx(0);
	}
	
	 
	Real Line::b() const
	{
		return yIntercept();
	}
	
	 
	Real Line::xIntercept() const
	{
		return fy(0);
	}
	
	 
	bool Line::visible(arVertex2D<Real> point) const
	{
		point -= offset;
		return (point.y * direction.x - point.x * direction.y) >= 0;
	}
	
	 
	bool Line::visibleByAll(const std::vector<Line>& lines, arVertex2D<Real> point)
	{
		for (const Line& line : lines)
			if (line.notVisible(point))
				return false;

		return true;
	}
	
	 
	bool Line::notVisible(arVertex2D<Real> point) const
	{
		return !visible(point);
	}
	
	 
	bool Line::notVisibleByAll(const std::vector<Line>& lines, arVertex2D<Real> point)
	{
		for (const Line& line : lines)
			if (line.visible(point))
				return false;

		return true;
	}
	
	 
	arVertex2D<Real> Line::intersect(const Line& other)
	{

		AR_ASSERT_MSG(!direction.perp(other.direction), "Cannot get intersecting point with parallel lines\ndirection: ", direction, ',', other.direction);

		// this works for all cases that has excactly one intersecting point
		// below is a system of the equations fx() / fy() and other.fx() / other.fy() solved for fx and fy

		Real fy = -(direction.x * other.direction.x * other.offset.y - direction.x * other.direction.x *  offset.y - direction.x * other.direction.y * other.offset.x + other.direction.x * direction.y * offset.x) / (direction.x * other.direction.y - other.direction.x * direction.y);
		Real fx = -(other.direction.x * direction.y * other.offset.y - direction.x * other.direction.y * offset.y - direction.y * other.direction.y * other.offset.x + direction.y * other.direction.y * offset.x) / (direction.x * other.direction.y - other.direction.x * direction.y);

		return { fy, fx };
	}
	
	// checks if the given point intersects the line
	// margin: the are around the line that should be considered to intersect the point
	 
	bool Line::intersects(arVertex2D<Real> point, Real margin)
	{
		AR_ASSERT_MSG(margin >= 0, "Margin cannot be less than 0\nMargin: ", margin);
		
		if (margin != 0)
		{
			// get unit vector pointing in the same direction as the line

			Vector<Real> line_vec = (1 / direction.norm()) * direction;

			// get the perpendicular vectors to line_vec with length of margin/2

			Vector<Real> perp_vec_l = { -line_vec.y, line_vec.x };
			perp_vec_l *= margin / 2;

			Vector<Real> perp_vec_r = { line_vec.y, -line_vec.x };
			perp_vec_r *= margin / 2;

			// offset the point by the two perpendicular vectors and the line offset

			arVertex2D<Real> point_l = point + perp_vec_l - offset;
			arVertex2D<Real> point_r = point + perp_vec_r - offset;

			// check wether both points are to the left or right of line_vec

			Real cross_l = point_l.y * line_vec.x - line_vec.y * point_l.x;
			Real cross_r = line_vec.y * point_r.x - point_r.y * line_vec.x;

			return cross_l >= 0 && cross_r >= 0;
		}
		else
		{
			return ((point.y - offset.y) * direction.x - direction.y * (point.x - offset.x)) == 0;
		}
	}


	// calls intersects with given arguments and also checks if the point is inside the direction vector of the line
	 
	bool Line::intDirection(arVertex2D<Real> point, Real margin)
	{
		bool intersects_line = intersects(point, margin);

		point -= offset;

		if (intersects_line)
			if (point.x > std::min(-margin, direction.x - margin) && point.x < std::max(margin, direction.x + margin) &&
				point.y > std::min(-margin, direction.y - margin) && point.y < std::max(margin, direction.y + margin))
				return true;
			else
				return false;
		else
			return false;
	}

}
