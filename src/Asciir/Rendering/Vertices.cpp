#include "arpch.h"
#include "Vertices.h"

namespace Asciir
{
	arVertex::arVertex(TInt x, TInt y)
	: x(x), y(y) {}

	arVertex::arVertex(const arBigVertex& derived)
		: x(derived.x), y(derived.y/2) {}

	arVertex::arVertex(const arSmallVertex& derived)
		: x(derived.x / 2), y(derived.y) {}

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

	arBigVertex::arBigVertex(TInt x, TInt y)
		:arVertex(x, y)
	{
	}

	arBigVertex::arBigVertex(const arVertex& base)
		: arVertex(base) {
		std::cout << "TEST\n";
	}

	arSmallVertex::arSmallVertex(TInt x, TInt y)
		: arVertex(x, y) {}

	arSmallVertex::arSmallVertex(const arVertex& base)
		: arVertex(base) {}

}
