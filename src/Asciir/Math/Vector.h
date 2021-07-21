#pragma once
#include "Vertices.h"

namespace Asciir
{
	template<typename T, size_t d>
	class arVector : public arVertex<T, d>
	{
	public:
		using arVertex<T, d>::arVertex;

		bool perp(const arVector<T, d>& other);
	};

	template<typename T>
	using Vector = arVector<T, 2>;

	typedef Vector<Real> RealVector;
}

#include "Vector.ipp"
