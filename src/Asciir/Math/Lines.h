#pragma once

#include "Vector.h"

namespace Asciir
{
	class LineSegment;
	
	/// @brief class representing a line on a 2D plane
	///
	/// represented as 2 vectors, a direction vector and a offset vector
	/// 
	class Line
	{
	public:
		Vector<Real> direction, offset;

		/// @brief Default constructor
		/// constructs a line with no direction and with offset (0, 0)
		Line() = default;
		// constructs a line from a linear equation y = ax + b
		Line(Real a, Real b);
		/// @brief constructs a line from an direction and offset vector
		/// @param direction the direction of the line
		/// @param offset the offset from the origin of the line
		Line(Vector<Real> direction, Vector<Real> offset = Vector<Real>());
		/// @brief Copy constructor
		Line(const Line& other);

		/// @brief converts the line to a linesegment of itself
		/// the linesegment will have the length of the direction vector and the offset of the offset vector
		operator LineSegment();

		/// @brief constructs a line from two points
		static Line fromPoints(arVertex2D<Real> a, arVertex2D<Real> b);
		/// @brief constructs a horizontal line with offset at p, pointing to the right, and a length of 1
		static Line horzLine(arVertex2D<Real> p);
		/// @brief constructs a vertical line with offset at p, upwards direction, and a length of 1
		static Line verticLine(arVertex2D<Real> p);

		/// @brief retrieve the corresponding y position from the given x position
		Real fx(Real x) const;
		/// @brief retrieve the corrseponding x position from the given y position
		Real fy(Real y) const;

		/// @brief retrieves a point from a pixelated version of the line
		/// 
		/// the line is first split into squares that have at least one sidelength be equal to resolution, and at most one sidelength less than.  
		/// afterwards the n'tn square in the direction of the line, where the origin is at indx 0, bottom left coordinate is retrieved and returned.
		/// 
		/// @param indx the square indx
		/// @param resolution the resolution of the pixelated line
		Coord pointFromGrid(long long indx, Real resolution);

		/// @brief returns the slope of the line  
		/// 
		/// simply returns a from the linear equation y = ax + b  
		/// 
		/// functions the same as a()
		/// 
		Real slope() const;
		/// @brief returns a from the linear equation describing the line (y = ax + b)
		Real a() const;
		/// @brief returns the y intercept of the line
		/// 
		/// returns b from the linear equation y = ax + b
		/// 
		/// functions the same as b()
		/// 
		Real yIntercept() const;
		/// @brief returns b from the linear equation describing the line (y = ax + b)
		Real b() const;
		/// @brief rethrns the x intercept of the line
		/// 
		/// equivalent to fy(0)
		/// 
		Real xIntercept() const;

		/// @brief returns wether the point is "visible" from the line
		/// 
		/// a point is percieved as "visible" if it is to the left of the offset direction vector.
		/// 
		/// visibility is calculated as  
		/// 
		//  AsciiMath:
		//                                   ^
		//  visible = (point - offset) * direction >= 0
		//  
		//	Latex:
		/// \f$ visible = (\overrightarrow{point}-\overrightarrow{offset}) \cdot \widehat{\overrightarrow{direction}} \geq 0 \f$
		/// 
		/// @param point point to check wether is visible
		bool visible(arVertex2D<Real> point) const;

		/// @brief simply checks if visible is true for all lines.  
		/// @param lines list of lines to be checked
		/// @param point point to be checked against
		template<typename T, std::enable_if_t<is_list_type_v<Line, T>, bool> = false>
		static bool visibleByAll(const T& lines, arVertex2D<Real> point);

		/// @brief returns wether the point is not "visible" from the line
		/// 
		/// same as !visible()
		bool notVisible(arVertex2D<Real> point) const;
		/// @brief checks if notVisible() is true for all the lines passed
		/// @param lines list of lines to be checked 
		/// @param point point to be checked against
		template<typename T, std::enable_if_t<is_list_type_v<Line, T>, bool> = false>
		static bool notVisibleByAll(const T& lines, arVertex2D<Real> point);

		/// @brief checks if the current line is perpendicular to the other line
		/// @param other line to check if perpendicular with
		bool isPerpendicular(const Line& other);

		/// @brief get the intersect point between current line and other line
		/// 
		/// only works if line and other are not parallel
		arVertex2D<Real> intersect(const Line& other);
		/// @brief checks if the current line intersects the passed point within the given margin.
		/// @param point point to check wether the line intersects
		/// @param margin buffer width added to the line that will be considered as "inside" the line
		bool intersects(arVertex2D<Real> point, Real margin = 0);
	};

	
	// 
	// 
	
	/// @brief A Line class which takes the length and position of the line into account. 
	/// The length of the direction vector represents the length of the segment
	class LineSegment : public Line
	{
	public:
		using Line::Line;

		/// @brief constructs a line segment from a line.
		LineSegment(const Line& other) : Line(other) {};
		/// @brief copy constructor
		LineSegment(const LineSegment& other) : Line((Line)other) {};

		/// @brief constructs a line segment that goes form point a to point b
		static LineSegment fromPoints(arVertex2D<Real> a, arVertex2D<Real> b) { return Line::fromPoints(a, b); };

		/// @brief get the point where the current line segment intersects with the passed line.  
		/// 
		/// if the line segment and the line does not intersect, this will assert.
		/// use intersects() to prevent this.
		arVertex2D<Real> intersect(const Line& other);
		/// @brief get the point where the current line segment intersects with the other line segmetn
		///
		/// same limitations as intersects(const Line&);
		arVertex2D<Real> intersect(const LineSegment& other);

		// TODO: is this needed?
		bool isPerpendicular(const Line& other);

		/// @brief returns wether the line segment intersects the point within the given margin
		/// 
		/// this function will check if the point is inside the rectangle formed by the line and the added margin to the line.  
		/// this also means the margin does not affect the end of the line segment.
		/// 
		/// @param point point to check if intersects with
		/// @param margin margin which the point will be considered to insersect within.
		bool intersects(arVertex2D<Real> point, Real margin = 0);
		/// @brief returns wether the line segment intsersects witht the line 
		bool intersects(const Line& other);
		/// @brief returns wether the line segment intsersects witht the line segment 
		bool intersects(const LineSegment& other);
	};
}

#include "Lines.ipp"
