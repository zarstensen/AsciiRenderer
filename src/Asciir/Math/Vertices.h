#pragma once

#include "Asciir/Core/core.h"

namespace Asciir
{
	// A wrapper for Vector2 that exposes index 0 and 1 in the Vector2 as member variables x and y.
	// It also has diffrent cout overloads and initializes x and y with 0 in the default constructor.
	template<typename T>
	struct arVertex : public Eigen::Vector2<T>
	{
		T& x;
		T& y;

		arVertex(T x, T y);
		arVertex();
		template<typename TOther>
		arVertex(const arVertex<TOther>& other);

		#ifdef AR_WIN
		arVertex(POINT point);
		#endif

		arVertex(const Eigen::Vector2<T>& vec);
		
		// constructors for eigen generic expressions

		template<typename TOther>
		arVertex(const Eigen::EigenBase<TOther>& other);
		template<typename TOther>
		arVertex(const Eigen::ReturnByValue<TOther>& other);

		arVertex<T> operator=(const arVertex<T>& other);

		#ifdef AR_WIN
		operator POINT();
		#endif

		using Eigen::Vector2<T>::operator[];
		using Eigen::Vector2<T>::operator();

	};

	// typedef for arVertex
	// s_ = static size arVertex

	template<typename T>
	using arVertices = std::vector<arVertex<T>>;
	
	template<typename T, size_t n>
	using s_arVertices = std::array<arVertex<T>, n>;

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T>& vert);
	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T>& verts);

	typedef arVertex<size_t> Size2D;

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

#include "Vertices.ipp"
