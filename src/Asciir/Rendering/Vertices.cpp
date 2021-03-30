#include "arpch.h"
#include "Vertices.h"

namespace Asciir
{
	arVertex::arVertex(TInt x, TInt y)
	: x(x), y(y) {}

	TInt& arVertex::coord(bool index)
	{
		if (index)
			return x;
		else
			return y;
	}

	TInt arVertex::coord(bool index) const
	{
		if (index)
			return x;
		else
			return y;
	}

	std::ostream& operator<<(std::ostream& stream, const arVertex& vert)
	{
		stream << '(' << vert.x << ',' << vert.y << ')';
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const arVertices& verts)
	{
		for (size_t i = 0; i < verts.size() - 1; i++)
		{
			stream << verts[i] << ',';
		}

		stream << verts[verts.size() - 1];

		return stream;
	}

}
