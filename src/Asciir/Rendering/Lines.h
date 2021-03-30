#pragma once

#include "Asciir/Rendering/Vertices.h"

namespace Asciir
{
	struct LineSegment
	{
		arVertex a, b;

		LineSegment() = default;
		LineSegment(arVertex a, arVertex b);

		TInt length();

		arVertex at(TInt pos);

		TInt getX(TInt y);
		TInt getY(TInt x);

		bool intersects(arVertex point);
	};

}
