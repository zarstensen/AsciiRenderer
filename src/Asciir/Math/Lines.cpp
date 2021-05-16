#include "arpch.h"
#include "Lines.h"

namespace Asciir
{
	LineSegment::LineSegment(TermVert a, TermVert b)
		: a(a), b(b) {}

	TInt LineSegment::length()
	{
		TermVert dvert = b - a;

		// return height if height is greater than width, otherwise return width
		if (std::abs(dvert.x) >= std::abs(dvert.y))
			return std::abs(dvert.x);	
		else
			return std::abs(dvert.y);
	}

	TermVert LineSegment::at(TInt pos)
	{
		TermVert dvert = b - a;

		// slope inbetween 1 and -1
		if (std::abs(dvert.x) >= std::abs(dvert.y))
		{
			float slope = float(dvert.y) / float(dvert.x);

			// flip points if a is to the left of b
			if (dvert.x > 0)
				return { a.x + pos, TInt(a.y + std::round(((float)pos) * slope)) };
			else
				return { b.x + pos, TInt(b.y + std::round(((float)pos) * slope)) };
		}
		else
		{
			float slope = float(dvert.x) / float(dvert.y);

			// flip points if a is under b
			if (dvert.y > 0)
				return { TInt( a.x + std::round(float(pos) * slope)),	a.y + pos };
			else
				return { TInt(b.x + std::round(((float)pos) * slope)),	b.y + pos };
		}
	}

	TInt LineSegment::getX(TInt y)
	{
		TermVert dvert = b - a;

		float slope = float(dvert.x) / float(dvert.y);
		return TInt(slope * y + a.x);
	}

	TInt LineSegment::getY(TInt x)
	{
		TermVert dvert = b - a;

		float slope = float(dvert.y) / float(dvert.x);
		return TInt(slope * x + a.y);
	}

	bool LineSegment::intersects(TermVert point)
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
}
