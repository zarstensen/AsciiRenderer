#pragma once

#include "Asciir/Core/core.h"

namespace Asciir
{

	template<typename>
	struct arBigVertex;
	template<typename>
	struct arSmallVertex;

	template<typename T>
	struct arVertex
	{
		T x = 0;
		T y = 0;

		arVertex() = default;
		arVertex(T x, T y);
		arVertex(const arBigVertex<T>& derived);
		arVertex(const arSmallVertex<T>& derived);
		template<typename TOther>
		arVertex(const arVertex<TOther>& other);

		arVertex<T> operator+(const arVertex<T>& other) const;
		arVertex<T> operator-(const arVertex<T>& other) const;
		arVertex<T> operator*(const arVertex<T>& other) const;
		arVertex<T> operator/(const arVertex<T>& other) const;
		arVertex<T> operator%(const arVertex<T>& other) const;

		void operator+=(const arVertex<T>& other);
		void operator-=(const arVertex<T>& other);
		void operator*=(const arVertex<T>& other);
		void operator/=(const arVertex<T>& other);
		void operator%=(const arVertex<T>& other);
	};

	template<typename T>
	struct arBigVertex: public arVertex<T>
	{
		arBigVertex() = default;
		arBigVertex(T x, T y);
		arBigVertex(const arVertex<T>& base);
	};

	template<typename T>
	struct arSmallVertex : public arVertex<T>
	{
		arSmallVertex() = default;
		arSmallVertex(T x, T y);
		arSmallVertex(const arVertex<T>& base);
	};

	template<typename T>
	using arVertices = std::vector<arVertex<T>>;
	
	template<typename T, size_t n>
	using s_arVertices = std::array<arVertex<T>, n>;

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T>& vert);
	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T>& verts);

	typedef arVertex<Real> RealVertex;
	typedef arVertices<Real> RealVertices;
	template<size_t n>
	using s_RealVertices = s_arVertices<Real, n>;

	typedef arVertex<int> Vertex;
	typedef arVertices<int> Vertices;
	template<size_t n>
	using s_Vertices = s_arVertices<int, n>;
	
	typedef arVertex<TInt> TermVert;
	typedef arVertices<TInt> TermVerts;
	template<size_t n>
	using s_TermVerts = s_arVertices<TInt, n>;
	
	typedef arVertex<long long> Coord;
	typedef arVertices<long long> Coords;
	template<size_t n>
	using s_Coords = s_arVertices<long long, n>;

}

