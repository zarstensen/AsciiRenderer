#pragma once

#include "Asciir/Core/core.h"

namespace Asciir
{
	struct arVertex
	{
		TInt x = 0;
		TInt y = 0;

		arVertex() = default;
		arVertex(TInt x, TInt y);

		TInt& coord(bool index);
		TInt coord(bool index) const;
	};

	typedef std::vector<arVertex> arVertices;

	std::ostream& operator<<(std::ostream& stream, const arVertex& vert);
	std::ostream& operator<<(std::ostream& stream, const arVertices& verts);
}

