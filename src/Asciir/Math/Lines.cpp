#include "pch/arpch.h"
#include "Lines.h"
#include "Vector.h"

namespace Asciir
{
	Line::Line(Real a, Real b)
		: direction(1, a), offset(0, b) {}
	
	 
	Line::Line(Vector<Real> direction, Vector<Real> offset)
		: direction(direction), offset(offset) {}

	Line::Line(const Line& other)
		: Line(other.direction, other.offset) {}

	Line::operator LineSegment()
	{
		return LineSegment(direction, offset);
	}
	 
	Line Line::fromPoints(arVertex2D<Real> a, arVertex2D<Real> b)
	{
		return Line(b - a, a);
	}

	Line Line::horzLine(arVertex2D<Real> p)
	{
		return Line::fromPoints(p, p + arVertex2D<Real>(1, 0));
	}

	Line Line::verticLine(arVertex2D<Real> p)
	{
		return Line::fromPoints(p, p + arVertex2D<Real>(0, 1));
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
	
	 
	bool Line::notVisible(arVertex2D<Real> point) const
	{
		return !visible(point);
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
	// margin: the area around the line that should be considered to intersect the point
	 
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

	bool Line::isPerpendicular(const Line& other)
	{
		return direction.perp(other.direction);
	}

	// if the lines does not intersect but is called anyways behavior is as follows
	// DEBUG: asserts
	// RELEASE: returns intersecting point as if the line segment was a line
	arVertex2D<Real> LineSegment::intersect(const Line& other)
	{
		AR_ASSERT_MSG(intersects(other), "Cannot find intersecting point if the lines do not intersect");
		AR_ASSERT_MSG(!isPerpendicular(other), "Cannot find intersecting point if the lines are perpendicular");
		return Line::intersect(other);
	}

	// if the lines does not intersect but is called anyways behavior is as follows
	// DEBUG: asserts
	// RELEASE: returns intersecting point as if the line segment was a line
	arVertex2D<Real> LineSegment::intersect(const LineSegment& other)
	{
		AR_ASSERT_MSG(intersects(other), "Cannot find intersecting point if the lines do not intersect");
		AR_ASSERT_MSG(!isPerpendicular(other), "Cannot find intersecting point if the lines are perpendicular");
		return Line::intersect(other);
	}

	bool LineSegment::isPerpendicular(const Line& other)
	{
		return direction.perp(other.direction);
	}

	bool LineSegment::intersects(arVertex2D<Real> point, Real margin)
	{
		bool intersects_line = Line::intersects(point, margin);

		point -= offset;

		if (intersects_line)
			if (point.x >= std::min(-margin, direction.x - margin) && point.x <= std::max(margin, direction.x + margin) &&
				point.y >= std::min(-margin, direction.y - margin) && point.y <= std::max(margin, direction.y + margin))
				return true;
			else
				return false;
		else
			return false;
	}

	bool LineSegment::intersects(const Line& other)
	{

		if (isPerpendicular(other))
		{
			if (direction.perp(offset - other.offset))
				return true;
			else
				return false;
		}

		arVertex2D<Real> intersect = Line::intersect(other);

		// check if point lays on the line segment

		arVertex2D<Real> distance_to_offset = offset - intersect;
		Real length_diff = distance_to_offset.norm() - direction.norm();
		bool are_same_dir = distance_to_offset.dot(direction) <= 0;

		return are_same_dir && length_diff <= 0;
	}

	bool LineSegment::intersects(const LineSegment& other)
	{

		// special case for perpendicular lines
		if (isPerpendicular(other))
		{

			if (direction.perp(offset - other.offset))
			{
				// check if the lines intersect each other

				arVertex2D<Real> dist_end_ostart = direction + offset - other.offset;
				arVertex2D<Real> dist_end_oend = direction + offset - other.direction + other.offset;
				arVertex2D<Real> dist_oend_start = other.direction + other.offset - offset;
				arVertex2D<Real> dist_oend_end = other.direction + other.offset - direction + offset;

				// if the vectors point in diffrent directions the value will be negative
				Real dot_oend = dist_oend_start.dot(dist_oend_end);
				Real dot_end = dist_end_ostart.dot(dist_end_oend);

				
				return dot_oend < 0 || dot_end < 0;
			}
			else
				return false;
		}

		arVertex2D<Real> intersect = Line::intersect(other);

		// check if point lays on this line segment

		arVertex2D<Real> distance_to_offset = offset - intersect;
		Real length_diff = distance_to_offset.norm() - direction.norm();
		bool are_same_dir = distance_to_offset.dot(direction) < 0;

		// check if point lays on other line segment

		arVertex2D<Real> other_distance_to_offset = other.offset - intersect;
		Real other_length_diff = other_distance_to_offset.norm() - other.direction.norm();
		bool other_are_same_dir = other_distance_to_offset.dot(other.direction) < 0;

		return are_same_dir && length_diff < 0 && other_are_same_dir && other_length_diff < 0;
	}
}
