#include "Vertices.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	// Vertex implementation

	template<typename T, size_t n>
	template<typename TOther>
	arVertBase<T, n>::arVertBase(const arVertBase<TOther, n>& other)
		: arVertBase(Eigen::Vector<T, n>(other.template cast<T>())) {}

	template<typename T, size_t n>
	arVertBase<T, n>::arVertBase(const Eigen::Vector<T, n>& vec)
		: Eigen::Vector<T, n>(vec) {}

	template<typename T, size_t n>
	template<typename TOther>
	arVertBase<T, n>& arVertBase<T, n>::operator=(const arVertBase<TOther, n>& other)
	{
		return arVertBase<T, n>(other);
	}

	template<typename T, size_t n>
	template<typename TOther>
	arVertBase<T, n>::arVertBase(const Eigen::MatrixBase<TOther>& other)
		: arVertBase(other.eval().template cast<T>()) {}

	template<typename T, size_t n>
	template<typename TOther>
	arVertBase<T, n>::arVertBase(const Eigen::ArrayBase<TOther>& other)
		: arVertBase(other.matrix().eval().template cast<T>()) {}

	template<typename T>
	arVertex<T, 2>::arVertex()
		: arVertex(0, 0) {}

	template<typename T>
	arVertex<T, 2>::arVertex(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	template<typename T>
	arVertex<T, 2>::arVertex(const arVertex<T, 2>& other)
		: arVertex(other.x, other.y) {}

	template<typename T>
	template<typename TOther>
	arVertex<T, 2>::arVertex(const arVertex<TOther, 2>& other)
		: arVertex((T)other.x, (T)other.y) {}

	template<typename T>
	arVertex<T, 2>& Asciir::arVertex<T, 2>::operator=(const arVertex<T, 2>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

#ifdef AR_WIN

	template<typename T>
	arVertex<T, 2>::arVertex(POINT point)
		: arVertex((Real)point.x, (Real)point.y) {}

	template<typename T>
	arVertex<T, 2>::operator POINT()
	{
		return { (LONG)x, (LONG)y };
	}

#endif

	template<typename T>
	arVertex<T, 3>::arVertex()
		: arVertex(0, 0, 0) {}

	template<typename T>
	arVertex<T, 3>::arVertex(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	arVertex<T, 3>::arVertex(const arVertex<T, 3>& other)
		: arVertex(other.x, other.y, other.z) {}

	template<typename T>
	template<typename TOther>
	arVertex<T, 3>::arVertex(const arVertex<TOther, 3>& other)
		: arVertex((T)other.x, (T)other.y, (T)other.z) {}

	template<typename T>
	arVertex<T, 3>& Asciir::arVertex<T, 3>::operator=(const arVertex<T, 3>& other)
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
		: arVertices(other.template cast<arVertex<T, d>>()) {}

	template<typename T, size_t d>
	arVertices<T, d>::arVertices(size_t length)
		: Eigen::VectorX<arVertex<T, d>>(length)
	{
		fill(arVertex<T, d>());
	}

	template<typename T, size_t d>
	arVertices<T, d>::arVertices(const Eigen::VectorX<arVertex<T, d>>& other)
		: Eigen::VectorX<arVertex<T, d>>(other) {}

	template<typename T, size_t d>
	template<typename TOther>
	arVertices<T, d>::arVertices(const Eigen::MatrixBase<TOther>& other)
		: arVertices(Eigen::VectorX<arVertex<T, d>>(other.eval().template cast<arVertex<T, d>>())) {}

	template<typename T, size_t d>
	arVertices<T, d>::arVertices(const std::initializer_list<arVertex<T, d>>& other)
		: Eigen::VectorX<arVertex<T, d>>{ other } {}

	template<typename T, size_t d>
	arVertices<T, d> arVertices<T, d>::offset(const arVertex<T, d>& vec)
	{
		arVertices<T, d>& res = *this;

		for (arVertex<T, d>& vert : res) vert += vec;

		return res;
	}

	template<typename T, size_t d>
	void arVertices<T, d>::fill(arVertex<T, d> val)
	{
		for (arVertex<T, d>& elem : *this)
			elem = val;
	}

	template<typename T, size_t d>
	void arVertices<T, d>::cResize(size_t size)
	{
		Eigen::VectorX<arVertex<T, d>>::conservativeResize(size);
	}

	template<typename T, size_t d>
	size_t arVertices<T, d>::size() const
	{
		return Eigen::VectorX<arVertex<T, d>>::size();
	}

	template<typename T, size_t d, size_t n>
	template<typename TOther>
	s_arVertices<T, d, n>::s_arVertices(const s_arVertices<TOther, d, n>& other)
		: s_arVertices(other.template cast<arVertex<T, d>>()) {}

	template<typename T, size_t d, size_t n>
	s_arVertices<T, d, n>::s_arVertices(const Eigen::Vector<arVertex<T, d>, n>& other)
		: Eigen::VectorX<arVertex<T, d>>(other) {}

	template<typename T, size_t d, size_t n>
	template<typename TOther>
	s_arVertices<T, d, n>::s_arVertices(const Eigen::MatrixBase<TOther>& other)
		: s_arVertices<T, d, n>(other.eval().template cast<arVertex<T, d>>()) {}

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

	template<typename T, size_t d, size_t n>
	void s_arVertices<T, d, n>::fill(arVertex<T, d> val)
	{
		for (arVertex<T, d>& elem : *this)
			elem = val;
	}

	template<typename T, size_t d, size_t n>
	constexpr size_t s_arVertices<T, d, n>::size() const
	{
		return n;
	}

	// vertsView class

	template<typename T, size_t d>
	template<typename TArr, std::enable_if_t<is_vertices_type_v<T, d, TArr>, bool>>
	VertsView<T, d>::VertsView(const TArr& source)
		: m_data(source.data()), m_len(source.size()) {}

	template<typename T, size_t d>
	template<typename TArr, std::enable_if_t<is_vertices_type_v<T, d, TArr>, bool>>
	VertsView<T, d>::VertsView(const TArr& source, size_t len, size_t offset)
		: m_data(source.data() + offset), m_len(len)
	{
		AR_ASSERT(source.size() <= len + offset);
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
		if (verts.size() > 0)
		{
			for (size_t i = 0; i < (size_t)verts.size() - 1; i++)
				stream << verts[i] << ',';
			
			stream << verts[verts.size() - 1];
		}
		else
			stream << "empty";

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