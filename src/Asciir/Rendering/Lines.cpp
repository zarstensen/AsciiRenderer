#include "arpch.h"
#include "Lines.h"

namespace Asciir
{
	LineSegment::LineSegment(arVertex a, arVertex b)
		: a(a), b(b) {}

	TInt LineSegment::length()
	{
		arVertex dvert = { b.x - a.x , b.y - a.y };

		if (std::abs(dvert.x) >= std::abs(dvert.y))
			return std::abs(dvert.x);	
		else
			return std::abs(dvert.y);
	}

	arVertex LineSegment::at(TInt pos)
	{
		arVertex dvert = { b.x - a.x , b.y - a.y };

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
				return { TInt(a.x + std::round(float(pos) * slope)), a.y + pos };
			else
				return { TInt(b.x + std::round(((float)pos) * slope)), b.y + pos };
		}
	}

	// TODO: fix this
	TInt LineSegment::getX(TInt y)
	{
		arVertex dvert = { b.x - a.x , b.y - a.y };

		float slope = float(dvert.x) / float(dvert.y);
		return slope * y + a.x;
	}

	TInt LineSegment::getY(TInt x)
	{
		arVertex dvert = { b.x - a.x , b.y - a.y };

		float slope = float(dvert.y) / float(dvert.x);
		return slope * x + a.y;
	}

	bool LineSegment::intersects(arVertex point)
	{
		arVertex vec_a = { b.x - a.x, b.y - a.y };
		arVertex vec_b = { b.x - point.x, b.y - point.y };

		// return false if point is not inside line bounding box
		if (vec_a.x > vec_b.x && vec_b.x < NULL || vec_a.y > vec_b.y && vec_b.y < NULL)
			return false;
		
		// return true if vec_b is a null vector
		if (vec_b.x == NULL && vec_b.y == NULL)
			return true;

		// check if the two vectors are parallel
		return std::abs(vec_a.x * vec_b.y) == std::abs(vec_b.x * vec_a.y);
	}
}
