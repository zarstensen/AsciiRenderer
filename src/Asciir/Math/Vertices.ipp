#include "Vertices.h"
#include <iostream>

namespace Asciir
{

	static arVertex2D<int> test = Eigen::Vector<int, 2>(100, 100);

	template<typename T, size_t n>
	template<typename TOther>
	arVertex<T, n>::arVertex(const arVertex<TOther, n>& other)
		: arVertex((T)other.x, (T)other.y) {}

	template<typename T, size_t n>
	arVertex<T, n>::arVertex(const Eigen::Vector<T, n>& vec)
		: Eigen::Vector<T, n>(vec) {}

	template<typename T, size_t n>
	template<typename TOther>
	arVertex<T, n>& arVertex<T, n>::operator=(const arVertex<TOther, n>& other)
	{

		return arVertex<T, n>(other);
	}

	template<typename T, size_t n>
	template<typename TOther>
	arVertex<T, n>::arVertex(const Eigen::MatrixBase<TOther>& other)
		: arVertex(other.template cast<T>()) {}

	template<typename T>
	arVertex2D<T>::arVertex2D()
		: arVertex2D(0, 0) {}

	template<typename T>
	arVertex2D<T>::arVertex2D(const T& x, const T& y)
		: arVertex<T, 2>(x, y), x(arVertex<T, 2>::operator[](0)), y(arVertex<T, 2>::operator[](1)) {}

	template<typename T>
	template<typename TOther>
	arVertex2D<T>::arVertex2D(const arVertex2D<TOther>& other)
		: arVertex2D((T)other.x, (T)other.y) {}

	template<typename T>
	arVertex2D<T>& Asciir::arVertex2D<T>::operator=(const arVertex2D<T>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	#ifdef AR_WIN
	
	template<typename T>
	arVertex2D<T>::arVertex2D(POINT point)
		: arVertex2D<T>(point.x, point.y){}

	template<typename T>
	arVertex2D<T>::operator POINT()
	{
		return { x, y };
	}

	#endif


	template<typename T>
	arVertex3D<T>::arVertex3D()
		: arVertex3D(0, 0, 0) {}

	template<typename T>
	arVertex3D<T>::arVertex3D(const T& x, const T& y, const T& z)
		: arVertex<T, 3>(x, y, z), x(arVertex<T, 3>::operator[](0)), y(arVertex<T, 3>::operator[](1)), z(arVertex<T, 3>::operator[](2)) {}

	template<typename T>
	template<typename TOther>
	arVertex3D<T>::arVertex3D(const arVertex3D<TOther>& other)
		: arVertex3D((T)other.x, (T)other.y, (T)other.z) {}

	template<typename T>
	arVertex3D<T>& Asciir::arVertex3D<T>::operator=(const arVertex3D<T>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		
		return *this;
	}


	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T, n>& vert)
	{
		stream << '(' << vert[0];

		for (size_t i = 1; i < n; i++)
			stream << ',' << vert[i];

		stream << ')';

		return stream;
	}

	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T, n>& verts)
	{
		for (size_t i = 0; i < verts.size() - 1; i++)
		{
			stream << verts[i] << ',';
		}

		stream << verts[verts.size() - 1];

		return stream;
	}
}
