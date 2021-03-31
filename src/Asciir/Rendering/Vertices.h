#pragma once

#include "Asciir/Core/core.h"

namespace Asciir
{

	struct arBigVertex;
	struct arSmallVertex;

	struct arVertex
	{
		TInt x = 0;
		TInt y = 0;

		arVertex() = default;
		arVertex(TInt x, TInt y);
		arVertex(const arBigVertex& derived);
		arVertex(const arSmallVertex& derived);
	};
	
	struct arBigVertex: public arVertex
	{
		arBigVertex() = default;
		arBigVertex(TInt x, TInt y);
		arBigVertex(const arVertex& base);
	};

	struct arSmallVertex : public arVertex
	{
		arSmallVertex() = default;
		arSmallVertex(TInt x, TInt y);
		arSmallVertex(const arVertex& base);
	};

	typedef std::vector<arVertex> arVertices;

	std::ostream& operator<<(std::ostream& stream, const arVertex& vert);
	std::ostream& operator<<(std::ostream& stream, const arVertices& verts);
}

