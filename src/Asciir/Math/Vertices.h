#pragma once

#include "Asciir/Core/Core.h"
#include <Eigen/Eigen>

namespace Asciir
{

	// A base wrapper to be used as a base for arVertex<n>, arVertex2D and arVertex3D
	
	template<typename T, size_t n>
	struct arVertBase : public Eigen::Vector<T, n>
	{
		arVertBase() = default;
		template<typename TOther>
		arVertBase(const arVertBase<TOther, n>&other);
		arVertBase(const Eigen::Vector<T, n>&vec);

		// constructors for eigen generic expressions
		template<typename TOther>
		arVertBase(const Eigen::MatrixBase<TOther>&other);

		template<typename TOther>
		arVertBase(const Eigen::ArrayBase<TOther>&other);

		template<typename TOther>
		arVertBase<T, n>& operator=(const arVertBase<TOther, n>&other);

		using Eigen::Vector<T, n>::operator[];
		using Eigen::Vector<T, n>::operator();
	};

	template<typename T, size_t n, class Enable = void>
	struct arVertex : public arVertBase<T, n>
	{
		using arVertBase<T, n>::arVertBase;
	};

	// exposes index 0 and 1 as member variables x and y if there is excactly 2 dimensions
	template<typename T>
	struct arVertex<T, 2, void> : public arVertBase<T, 2>
	{

		using arVertBase<T, 2>::arVertBase;

		T& x = arVertex<T, 2>::operator[](0);
		T& y = arVertex<T, 2>::operator[](1);


		arVertex();
		arVertex(T x, T y);

		// copy constructor
		arVertex(const arVertex<T, 2>& other);
		
		// cast constructors
		template<typename TOther>
		arVertex(const arVertex<TOther, 2>& other);
		arVertex<T, 2>& operator=(const arVertex<T, 2>& other);

		#ifdef AR_WIN
		arVertex(POINT point);
		operator POINT();
		#endif
	};


	// exposes index 0, 1 and 2 as member variables x, y and z if there are 3 dimensions
	template<typename T>
	struct arVertex<T, 3, void> : public arVertBase<T, 3>
	{
		using arVertBase<T, 3>::arVertBase;

		T& x = arVertex<T, 3>::operator[](0);
		T& y = arVertex<T, 3>::operator[](1);
		T& z = arVertex<T, 3>::operator[](2);

		arVertex();
		arVertex(T x, T y, T z);

		// copy constructor
		arVertex(const arVertex<T, 3>& other);

		// cast constructors
		template<typename TOther>
		arVertex(const arVertex<TOther, 3>& other);

		arVertex<T, 3>& operator=(const arVertex<T, 3>& other);

	};


	// exposes index 0, 1 and 2 as member variables x, y and z if there are more than 3 dimensions.
	// no special constructors are created
	template<typename T, size_t d>
	struct arVertex<T, d, std::enable_if_t<(d > 3)>> : public arVertBase<T, d>
	{
		using arVertBase<T, d>::arVertBase;

		T& x = arVertex<T, d>::operator[](0);
		T& y = arVertex<T, d>::operator[](1);
		T& z = arVertex<T, d>::operator[](2);
	};
	
	// wrapper for Eigen VectorX with arVertex as the data type
	
	template<typename T, size_t d>
	struct arVertices : public Eigen::VectorX<arVertex<T, d>>
	{
		arVertices() = default;

		arVertices(size_t length);

		template<typename TOther>
		arVertices(const arVertices<TOther, d>&other);
		arVertices(const Eigen::VectorX<arVertex<T, d>>&other);
		arVertices(const std::initializer_list<arVertex<T, d>>&other);

		template<typename TOther>
		arVertices(const Eigen::MatrixBase<TOther>&other);

		arVertices<T, d> offset(const arVertex<T, d>& vec);
		
		void fill(arVertex<T, d> val);

		// shortend version of conservativeResize
		void cResize(size_t size);

		size_t size() const;

		using Eigen::VectorX<arVertex<T, d>>::operator[];
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

		s_arVertices<T, d, n> offset(const arVertex<T, d>&vec);

		void fill(arVertex<T, d> val);

		constexpr size_t size() const;

		using Eigen::Vector<arVertex<T, d>, n>::operator[];
	};

	// typedef for arVertex
	// s_ = static size arVertex

	template<typename T>
	using arVertex2D = arVertex<T, 2>;

	template<typename T>
	using arVertex3D = arVertex<T, 3>;

	template<typename T>
	using arVertices2D = arVertices<T, 2>;

	template<typename T>
	using arVertices3D = arVertices<T, 3>;

	template<typename T, size_t n>
	using s_arVertices2D = s_arVertices<T, n, 2>;

	template<typename T, size_t n>
	using s_arVertices3D = s_arVertices<T, n, 3>;


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

	typedef arVertex2D<Real> Coord;
	typedef arVertex3D<Real> Coord3D;

	typedef arVertices2D<Real> Coords;
	typedef arVertices3D<Real> Coords3D;

	typedef Real Scale;
	typedef arVertex2D<Scale> Scale2D;
	typedef arVertex3D<Scale> Scale3D;

	template<size_t n>
	using s_Coords = s_arVertices2D<Real, n>;
	template<size_t n>
	using s_Coords3D = s_arVertices3D<Real, n>;

	template<typename T, typename TEST = void>
	struct is_vert
	{
		static constexpr bool value = false;
	};

	template<typename T, size_t d, typename TTest, template<typename T, size_t, typename> class TVert>
	struct is_vert<TVert<T, d, TTest>, std::enable_if_t<std::is_same_v<TVert<T, d, TTest>, arVertex<T, d>>>>
	{
		static constexpr bool value = true;
	};

	// template check for array or vector types
	template<typename T>
	struct is_vertices
	{
		static constexpr bool value = false;
	};

	template<typename T, size_t d>
	struct is_vertices<arVertices<T, d>>
	{
		static constexpr bool value = true;
	};

	template<typename T, size_t d, size_t n>
	struct is_vertices<s_arVertices<T, d, n>>
	{
		static constexpr bool value = true;
	};

	template<typename TStorage, size_t d_storage, typename T>
	struct is_vertices_type
	{
		static constexpr bool value = false;
	};

	template<typename TStorage, size_t d_storage, template<typename, size_t> class TArr>
	struct is_vertices_type<TStorage, d_storage, TArr<TStorage, d_storage>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage>>::value;
	};

	template<typename TStorage, size_t d_storage, template<typename, size_t, size_t> class TArr, size_t n>
	struct is_vertices_type<TStorage, d_storage, TArr<TStorage, d_storage, n>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage, n>>::value;
	};

	template<typename TStorage, typename T, typename TEST = void>
	struct is_vertices_vtype
	{
		static constexpr bool value = false;
	};

	template<typename TStorage, size_t d_storage, template<typename, size_t, typename> class TVert, template<typename, size_t> class TArr>
	struct is_vertices_vtype<TVert<TStorage, d_storage, void>, TArr<TStorage, d_storage>, std::enable_if_t<is_vert<TVert<TStorage, d_storage, void>>::value>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage>>::value;
	};

	template<typename TStorage, size_t d_storage, template<typename, size_t, typename> class TVert, template<typename, size_t, size_t> class TArr, size_t n>
	struct is_vertices_vtype<TVert<TStorage, d_storage, void>, TArr<TStorage, d_storage, n>, std::enable_if_t<is_vert<TVert<TStorage, d_storage, void>>::value>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage, n>>::value;
	};

	template<typename T>
	struct is_list
	{
		static constexpr bool value = is_vertices<T>::value;
	};

	template<typename T, size_t n>
	struct is_list<std::array<T, n>>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	struct is_list<std::vector<T>>
	{
		static constexpr bool value = true;
	};


	template<typename TStorage, typename T>
	struct is_list_type
	{
		static constexpr bool value = is_vertices_vtype<TStorage, T>::Value;
	};

	template<typename TStorage, template<typename ...> class T, typename ... Args>
	struct is_list_type<TStorage, T<TStorage, Args...>>
	{
		static constexpr bool value = is_list<T<TStorage>>::value;
	};

	template<typename TStorage, size_t n, template<typename, size_t, typename ...> class T, typename ... Args>
	struct is_list_type<TStorage, T<TStorage, n, Args...>>
	{
		static constexpr bool value = is_list<T<TStorage, n>>::value;
	};


	template<typename T>
	constexpr bool is_vert_v = is_vert<T>::value;
	template<typename T>
	constexpr bool is_vertices_v = is_vertices<T>::value;
	template<typename TStorage, size_t d, typename T>
	constexpr bool is_vertices_type_v = is_vertices_type<TStorage, d, T>::value;
	template<typename TStorage, typename T>
	constexpr bool is_vertices_vtype_v = is_vertices_vtype<TStorage, T>::value;
	template<typename T>
	constexpr bool is_list_v = is_list<T>::value;
	template<typename TStorage, typename T>
	constexpr bool is_list_type_v = is_list_type<TStorage, T>::value;

	// view class for verticies

	template<typename T, size_t d>
	class VertsView
	{
		const T* m_data;
		size_t m_len;
	public:

		template<typename TArr, std::enable_if_t<is_vertices_type_v<T, d, TArr>, bool> = false>
		VertsView(const TArr& source);

		template<typename TArr, std::enable_if_t<is_vertices_type_v<T, d, TArr>, bool> = false>
		VertsView(const TArr& source, size_t len, size_t offset = 0);

		const T& operator[](size_t i) { return m_data[i]; }

		size_t size() const { return m_len; }

		const T* begin() const { return m_data; }
		const T* end() const { return m_data + m_len; }

	};

	// ostream functions

	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T, n>& vert);
	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T, n>& verts);

}

#include "Vertices.ipp"
