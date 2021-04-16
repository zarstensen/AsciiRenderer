#pragma once

#include "Vertices.h"

namespace Asciir
{
	struct LineSegment
	{
		TermVert a, b;

		LineSegment() = default;
		LineSegment(TermVert a, TermVert b);

		TInt length();

		TermVert at(TInt pos);

		TInt getX(TInt y);
		TInt getY(TInt x);

		bool intersects(TermVert point);
	};

}
