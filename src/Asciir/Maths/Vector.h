#pragma once
#include "Vertices.h"

namespace Asciir
{
	/// @brief wrapper class for the arVertex class, providing functions often used with Vectors
	/// @tparam T datatype stored in the vector
	/// @tparam d the dimension of the vector
	template<typename T, size_t d>
	class arVector : public arVertex<T, d>
	{
	public:
		using arVertex<T, d>::arVertex;

		/// @brief checks wether the given vector is perpendicular to this
		bool perp(const arVector<T, d>& other);
	};

	template<typename T>
	using Vector = arVector<T, 2>;

	typedef Vector<Real> RealVector;
}

#include "Vector.ipp"
