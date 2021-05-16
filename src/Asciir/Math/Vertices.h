#pragma once

#include "Asciir/Core/Core.h"
#include <Eigen/Eigen>

namespace Asciir
{
	// A wrapper to be used as a base for arVertex2 and arVertex3
	template<typename T, size_t n>
	struct arVertex : public Eigen::Vector<T, n>
	{
		arVertex() = default;
		template<typename TOther>
		arVertex(const arVertex<TOther, n>& other);
		arVertex(const Eigen::Vector<T, n>& vec);
		
		// constructors for eigen generic expressions
		template<typename TOther>
		arVertex(const Eigen::MatrixBase<TOther>& other);

		template<typename TOther>
		arVertex<T, n>& operator=(const arVertex<TOther, n>& other);


		using Eigen::Vector<T, n>::operator[];
		using Eigen::Vector<T, n>::operator();

	};


	// exposes index 0 and 1 as member variables x and y
	template<typename T>
	struct arVertex2D : public arVertex<T, 2>
	{
		T& x = arVertex<T, 2>::operator[](0);
		T& y = arVertex<T, 2>::operator[](1);

		using arVertex<T, 2>::arVertex;

		arVertex2D();
		arVertex2D(T x, T y);

		// copy constructor
		arVertex2D(const arVertex2D<T>& other);
		
		// cast constructor
		template<typename TOther>
		arVertex2D(const arVertex2D<TOther>& other);

		arVertex2D<T>& operator=(const arVertex2D<T>& other);

		#ifdef AR_WIN
		arVertex2D(POINT point);
		operator POINT();
		#endif
	};


	// exposes index 0, 1 and 2 as member variables x, y and z
	template<typename T>
	struct arVertex3D: public arVertex<T, 3>
	{
		T& x = arVertex<T, 3>::operator[](0);
		T& y = arVertex<T, 3>::operator[](1);
		T& z = arVertex<T, 3>::operator[](2);

		using arVertex<T, 3>::arVertex;

		arVertex3D();
		arVertex3D(T x, T y, T z);

		// copy constructor
		arVertex3D(const arVertex3D<T>& other);

		// cast constructor
		template<typename TOther>
		arVertex3D(const arVertex3D<TOther>& other);

		arVertex3D<T>& operator=(const arVertex3D<T>& other);

	};
	
	// wrapper for Eigen VectorX with arVertex as the data type
	template<typename T, size_t d>
	struct arVertices : public Eigen::VectorX<arVertex<T, d>>
	{
		arVertices() = default;

		template<typename TOther>
		arVertices(const arVertices<TOther, d>& other);
		arVertices(const Eigen::VectorX<arVertex<T, d>>& other);
		arVertices(const std::initializer_list<arVertex<T, d>>& other);

		template<typename TOther>
		arVertices(const Eigen::MatrixBase<TOther>& other);

		/*arVertices<T, d>& operator=(const arVertices<T, d>& other);*/

		arVertices<T, d> offset(const arVertex<T, d>& vec);

		using Eigen::VectorX<arVertex<T, d>>::operator[];
		using Eigen::VectorX<arVertex<T, d>>::operator();
	};
	
	template<typename T, size_t d, size_t n>
	struct s_arVertices : public Eigen::Vector<arVertex<T, d>, n>
	{
		s_arVertices() = default;

		template<typename TOther>
		s_arVertices(const s_arVertices<TOther, d, n>&other);
		s_arVertices(const Eigen::Vector<arVertex<T, d>, n>&other);
		s_arVertices(const std::initializer_list<arVertex<T, d>>& other);

		template<typename TOther>
		s_arVertices(const Eigen::MatrixBase<TOther>& other);

		s_arVertices<T, d, n>& operator=(const s_arVertices<T, d, n>&other);

		s_arVertices<T, d, n> offset(const arVertex<T, d>&vec);

		using Eigen::VectorX<arVertex<T, d>>::operator[];
		using Eigen::VectorX<arVertex<T, d>>::operator();
	};

	// typedef for arVertex
	// s_ = static size arVertex

	template<typename T>
	using arVertices2D = arVertices<T, 2>;

	template<typename T>
	using arVertices3D = arVertices<T, 3>;

	template<typename T, size_t n>
	using s_arVertices2D = s_arVertices<T, n, 2>;

	template<typename T, size_t n>
	using s_arVertices3D = s_arVertices<T, n, 3 >;


	typedef arVertex2D<size_t> Size2D;
	typedef arVertex3D<size_t> Size3D;

	typedef arVertex2D<Real> RealVertex;
	typedef arVertex3D<Real> RealVertex3D;
	typedef arVertices2D<Real> RealVertices;
	typedef arVertices3D<Real> RealVertices3D;
	template<size_t n>
	using s_RealVertices = s_arVertices2D<Real, n>;
	template<size_t n>
	using s_RealVertices3D = s_arVertices3D<Real, n>;

	typedef arVertex2D<int> Vertex;
	typedef arVertex3D<int> Vertex3D;
	typedef arVertices2D<int> Vertices;
	typedef arVertices3D<int> Vertices3D;
	template<size_t n>
	using s_Vertices = s_arVertices2D<int, n>;
	template<size_t n>
	using s_Vertices3D = s_arVertices3D<int, n>;
	
	typedef arVertex2D<TInt> TermVert;
	typedef arVertices2D<TInt> TermVerts;
	template<size_t n>
	using s_TermVerts = s_arVertices2D<TInt, n>;
	
	typedef arVertex2D<long long> Coord;
	typedef arVertex3D<long long> Coord3D;
	typedef arVertices2D<long long> Coords;
	typedef arVertices3D<long long> Coords3D;
	template<size_t n>
	using s_Coords = s_arVertices2D<long long, n>;
	template<size_t n>
	using s_Coords3D = s_arVertices3D<long long, n>;


	// ostream functions

	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T, n>& vert);
	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T, n>& verts);
}

#include "Vertices.ipp"
