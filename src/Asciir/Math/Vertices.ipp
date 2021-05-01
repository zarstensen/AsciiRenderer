#include "Vertices.h"
#include <iostream>

namespace Asciir
{
	template<typename T>
	arVertex<T>::arVertex(T x, T y)
		: Eigen::Vector2<T>(x, y), x(operator[](0)), y(operator[](1)) {}

	template<typename T>
	arVertex<T>::arVertex()
		: arVertex(0, 0) {}

	template<typename T>
	template<typename TOther>
	arVertex<T>::arVertex(const arVertex<TOther>& other)
		: arVertex((T)other.x, (T)other.y) {}

	template<typename T>
	arVertex<T>::arVertex(POINT point)
		: arVertex(point.x, point.y) {}

	template<typename T>
	arVertex<T>::arVertex(const Eigen::Vector2<T>& vec)
		: arVertex(vec[0], vec[1]) {}

	template<typename T>
	arVertex<T> arVertex<T>::operator=(const arVertex<T>& other)
	{
		return arVertex<T>(other);
	}

	template<typename T>
	arVertex<T>::operator POINT()
	{
		return { x, y };
	}

	template<typename T>
	template<typename TOther>
	arVertex<T>::arVertex(const Eigen::EigenBase<TOther>& other)
		: arVertex(Eigen::Vector2<T>(other)) {}

	template<typename T>
	template<typename TOther>
	arVertex<T>::arVertex(const Eigen::ReturnByValue<TOther>& other)
		: arVertex(Eigen::Vector2<T>(other)) {}

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T>& vert)
	{
		stream << '(' << vert.x << ',' << vert.y << ')';
		return stream;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T>& verts)
	{
		for (size_t i = 0; i < verts.size() - 1; i++)
		{
			stream << verts[i] << ',';
		}

		stream << verts[verts.size() - 1];

		return stream;
	}
}
