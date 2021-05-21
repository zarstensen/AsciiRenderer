#include "Vertices.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{

	// Vertex implementation

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
		: arVertex(other.eval().template cast<T>()) {}

	template<typename T, size_t n>
	template<typename TOther>
	inline arVertex<T, n>::arVertex(const Eigen::ArrayBase<TOther>& other)
		: arVertex(other.matrix().eval().template cast<T>()) {}

	template<typename T>
	arVertex2D<T>::arVertex2D()
		: arVertex2D(0, 0) {}

	template<typename T>
	arVertex2D<T>::arVertex2D(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	template<typename T>
	arVertex2D<T>::arVertex2D(const arVertex2D<T>& other)
		: arVertex2D(other.x, other.y) {}

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
	arVertex3D<T>::arVertex3D(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	arVertex3D<T>::arVertex3D(const arVertex3D<T>& other)
		: arVertex3D(other.x, other.y, other.z) {}

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


	// Vertices implementation

	template<typename T, size_t d>
	template<typename TOther>
	arVertices<T, d>::arVertices(const arVertices<TOther, d>& other)
		: arVertices(other.template cast<T>()) {}


	template<typename T, size_t d>
	arVertices<T, d>::arVertices(const Eigen::VectorX<arVertex<T, d>>& other)
		: Eigen::VectorX<arVertex<T, d>>(other) {}

	template<typename T, size_t d>
	template<typename TOther>
	arVertices<T, d>::arVertices(const Eigen::MatrixBase<TOther>& other)
		: arVertices<T, d>(other.eval().template cast<T>()) {}

	template<typename T, size_t d>
	arVertices<T, d>::arVertices(const std::initializer_list<arVertex<T, d>>& other)
		: Eigen::VectorX<arVertex<T, d>>{ other } {}

	template<typename T, size_t d>
	arVertices<T, d> arVertices<T, d>::offset(const arVertex<T, d>& vec)
	{
		arVertices<T, d> res = *this;

		for (arVertex<T, d>& vert : res)
		{
			vert += vec;
		}

		return res;
	}

	
	template<typename T, size_t d, size_t n>
	template<typename TOther>
	s_arVertices<T, d, n>::s_arVertices(const s_arVertices<TOther, d, n>& other)
		: s_arVertices(other.template cast<T>()) {}


	template<typename T, size_t d, size_t n>
	s_arVertices<T, d, n>::s_arVertices(const Eigen::Vector<arVertex<T, d>, n>& other)
		: Eigen::VectorX<arVertex<T, d>>(other) {}

	template<typename T, size_t d, size_t n>
	template<typename TOther>
	s_arVertices<T, d, n>::s_arVertices(const Eigen::MatrixBase<TOther>& other)
		: s_arVertices<T, d, n>(other.eval().template cast<T>()) {}

	template<typename T, size_t d, size_t n>
	s_arVertices<T, d, n>::s_arVertices(const std::initializer_list<arVertex<T, d>>& other)
		: Eigen::Vector<arVertex<T, d>, n>{ other } {}

	template<typename T, size_t d, size_t n>
	s_arVertices<T, d, n> s_arVertices<T, d, n>::offset(const arVertex<T, d>& vec)
	{
		s_arVertices<T, d, n> res = *this;

		for (arVertex<T, d>& vert : res)
		{
			vert += vec;
		}

		return res;
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

	template<typename T, size_t d>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T, d>& verts)
	{
		for (size_t i = 0; i < verts.size() - 1; i++)
		{
			stream << verts[i] << ',';
		}

		stream << verts[verts.size() - 1];

		return stream;
	}

	template<typename T, size_t d, size_t n>
	std::ostream& operator<<(std::ostream& stream, const s_arVertices<T, d, n>& verts)
	{
		for (size_t i = 0; i < n - 1; i++)
		{
			stream << verts[i] << ',';
		}

		stream << verts[verts.size() - 1];

		return stream;
	}
}
