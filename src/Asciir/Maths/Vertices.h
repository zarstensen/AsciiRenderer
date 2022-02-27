#pragma once

#include "Asciir/Core/Core.h"
#include <Eigen/Eigen>

namespace Asciir
{
	/// @brief A base wrapper to be used as a base for arVertex<n>, arVertex2D and arVertex3D
	/// @tparam the datatype to be stored
	/// @tparam n the number of dimensions
	template<typename T, size_t n>
	struct arVertBase : public Eigen::Vector<T, n>
	{
		arVertBase() = default;
		template<typename TOther>
		/// @brief copy constructor
		arVertBase(const arVertBase<TOther, n>& other);
		/// @brief copy constructor for eigen type
		arVertBase(const Eigen::Vector<T, n>& vec);

		/// @brief constructors for eigen generic expressions
		template<typename TOther>
		arVertBase(const Eigen::MatrixBase<TOther>& other);
		/// @brief constructors for eigen generic expressions
		template<typename TOther>
		arVertBase(const Eigen::ArrayBase<TOther>& other);

		template<typename TOther>
		arVertBase<T, n>& operator=(const arVertBase<TOther, n>& other);

		using Eigen::Vector<T, n>::operator[];
		using Eigen::Vector<T, n>::operator();
	};

	template<typename T, size_t n, class Enable = void>
	struct arVertex : public arVertBase<T, n>
	{
		using arVertBase<T, n>::arVertBase;
	};

	/// @brief exposes index 0 and 1 as member variables x and y, if there is excactly 2 dimensions
	template<typename T>
	struct arVertex<T, 2, void> : public arVertBase<T, 2>
	{
		using arVertBase<T, 2>::arVertBase;

		T& x = arVertex<T, 2>::operator[](0);
		T& y = arVertex<T, 2>::operator[](1);

		arVertex();
		arVertex(T x, T y);

		/// @brief copy constructor
		arVertex(const arVertex<T, 2>& other);

		/// @brief cast constructor
		template<typename TOther>
		arVertex(const arVertex<TOther, 2>& other);
		arVertex<T, 2>& operator=(const arVertex<T, 2>& other);

#ifdef AR_WIN
		/// @brief constructor taking the windows POINT type.
		/// @attention this operator is windows specific, and should thus not be used within client code
		arVertex(POINT point);
		/// @brief cast operator for the windows POIN type.
		/// @attention this operator is windows specific, and should thus not be used within client code
		operator POINT();
#endif
	};

	/// @brief exposes index 0, 1 and 2 as member variables x, y and z if there are 3 dimensions
	template<typename T>
	struct arVertex<T, 3, void> : public arVertBase<T, 3>
	{
		using arVertBase<T, 3>::arVertBase;

		T& x = arVertex<T, 3>::operator[](0);
		T& y = arVertex<T, 3>::operator[](1);
		T& z = arVertex<T, 3>::operator[](2);

		arVertex();
		arVertex(T x, T y, T z);

		/// @brief copy constructor
		arVertex(const arVertex<T, 3>& other);

		/// @brief cast constructors
		template<typename TOther>
		arVertex(const arVertex<TOther, 3>& other);

		arVertex<T, 3>& operator=(const arVertex<T, 3>& other);
	};

	/// @brief exposes index 0, 1 and 2 as member variables x, y and z if there are more than 3 dimensions.
	/// @brief no special constructors are created
	template<typename T, size_t d>
	struct arVertex<T, d, std::enable_if_t<(d > 3)>> : public arVertBase<T, d>
	{
		using arVertBase<T, d>::arVertBase;
		
		T& x = arVertex<T, d>::operator[](0);
		T& y = arVertex<T, d>::operator[](1);
		T& z = arVertex<T, d>::operator[](2);
	};

	/// @brief wrapper for Eigen VectorX with arVertex as the data type
	/// @tparam T datatype of the vertices
	/// @tparam d dimension of the vertices
	template<typename T, size_t d>
	struct arVertices : public Eigen::VectorX<arVertex<T, d>>
	{
		arVertices() = default;

		/// @brief constructs a list of vertices of the specified length.
		/// @param length 
		arVertices(size_t length);

		/// @brief cast constructor
		template<typename TOther>
		arVertices(const arVertices<TOther, d>& other);
		/// @brief copy constructor for eigen base type
		arVertices(const Eigen::VectorX<arVertex<T, d>>& other);
		/// @brief initializer list constructor
		arVertices(const std::initializer_list<arVertex<T, d>>& other);

		/// @brief constructors for eigen generic expressions 
		template<typename TOther>
		arVertices(const Eigen::MatrixBase<TOther>& other);

		/// @brief offset all vertices by vec
		arVertices<T, d> offset(const arVertex<T, d>& vec);

		/// @brief set all vertices to val
		void fill(arVertex<T, d> val);

		/// @brief shortend version of conservativeResize
		void cResize(size_t size);

		/// @return number of vertices
		size_t size() const;

		using Eigen::VectorX<arVertex<T, d>>::operator[];
	};

	/// @brief staticly sized version of arVertices
	/// @tparam T datatype of the vertices
	/// @tparam d dimension of the vertices
	/// @tparam n number of vertices
	template<typename T, size_t d, size_t n>
	struct s_arVertices : public Eigen::Vector<arVertex<T, d>, n>
	{
		s_arVertices() = default;

		template<typename TOther>
		s_arVertices(const s_arVertices<TOther, d, n>& other);
		s_arVertices(const Eigen::Vector<arVertex<T, d>, n>& other);
		s_arVertices(const std::initializer_list<arVertex<T, d>>& other);

		template<typename TOther>
		s_arVertices(const Eigen::MatrixBase<TOther>& other);

		/// @see arVertices::offset()
		s_arVertices<T, d, n> offset(const arVertex<T, d>& vec);

		/// @see arVertices::fill()
		void fill(arVertex<T, d> val);

		/// @see arVertices::size()
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

	/// @brief compile time check if passed type is a vertex type
	/// @tparam T type to test
	template<typename T, typename TEST = void>
	struct is_vert
	{
		static constexpr bool value = false;
	};

	/// @see is_vert
	template<typename T, size_t d, typename TTest, template<typename T, size_t, typename> class TVert>
	struct is_vert<TVert<T, d, TTest>, std::enable_if_t<std::is_same_v<TVert<T, d, TTest>, arVertex<T, d>>>>
	{
		static constexpr bool value = true;
	};

	/// @brief compile time check if passed type is a vertices type
	template<typename T>
	struct is_vertices
	{
		static constexpr bool value = false;
	};

	/// @see is_vertices
	template<typename T, size_t d>
	struct is_vertices<arVertices<T, d>>
	{
		static constexpr bool value = true;
	};

	/// @see is_vertices
	template<typename T, size_t d, size_t n>
	struct is_vertices<s_arVertices<T, d, n>>
	{
		static constexpr bool value = true;
	};

	/// @brief compile time check if passed type is vertices, and also stores the passed vertex type
	/// @tparam TStorage the storage type of the storage vertex
	/// @tparam d_storage the dimensions of the storage vertex
	/// @tparam T the type to check
	template<typename TStorage, size_t d_storage, typename T>
	struct is_vertices_type
	{
		static constexpr bool value = false;
	};

	/// @see is_vertices_type
	template<typename TStorage, size_t d_storage, template<typename, size_t> class TArr>
	struct is_vertices_type<TStorage, d_storage, TArr<TStorage, d_storage>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage>>::value;
	};

	/// @see is_vertices_type
	template<typename TStorage, size_t d_storage, template<typename, size_t, size_t> class TArr, size_t n>
	struct is_vertices_type<TStorage, d_storage, TArr<TStorage, d_storage, n>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage, n>>::value;
	};

	/// @brief same as is_vertices_type, but a vertex type is passed, instead of the template arguments for the vertex type
	/// @tparam TStorage vertex type
	/// @tparam T type to check
	template<typename TStorage, typename T, typename TEST = void>
	struct is_vertices_vtype
	{
		static constexpr bool value = false;
	};

	/// @see is_vertices_vtype
	template<typename TStorage, size_t d_storage, template<typename, size_t, typename> class TVert, template<typename, size_t> class TArr>
	struct is_vertices_vtype<TVert<TStorage, d_storage, void>, TArr<TStorage, d_storage>, std::enable_if_t<is_vert<TVert<TStorage, d_storage, void>>::value>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage>>::value;
	};

	/// @see is_vertices_vtype
	template<typename TStorage, size_t d_storage, template<typename, size_t, typename> class TVert, template<typename, size_t, size_t> class TArr, size_t n>
	struct is_vertices_vtype<TVert<TStorage, d_storage, void>, TArr<TStorage, d_storage, n>, std::enable_if_t<is_vert<TVert<TStorage, d_storage, void>>::value>>
	{
		static constexpr bool value = is_vertices<TArr<TStorage, d_storage, n>>::value;
	};

	/// @brief checks if type is a list
	/// 
	/// a type is considered a list if:
	/// it is a vertices / s_vertices type
	/// it is an std::array type
	/// it is an std::vector type
	/// 
	/// @tparam T type to check
	template<typename T>
	struct is_list
	{
		static constexpr bool value = is_vertices<T>::value;
	};

	/// @see is_list
	template<typename T, size_t n>
	struct is_list<std::array<T, n>>
	{
		static constexpr bool value = true;
	};

	/// @see is_list
	template<typename T>
	struct is_list<std::vector<T>>
	{
		static constexpr bool value = true;
	};

	/// @brief checks if type is list, and stores a specific datatype.  
	/// 
	/// see is_list to get a list of types that qualifies as a list.
	/// 
	/// @tparam TStorage the datatype the list should store
	/// @tparam T the type to be checked
	template<typename TStorage, typename T>
	struct is_list_type
	{
		static constexpr bool value = is_vertices_vtype<TStorage, T>::Value;
	};

	/// @see is_list_type
	template<typename TStorage, template<typename ...> class T, typename ... Args>
	struct is_list_type<TStorage, T<TStorage, Args...>>
	{
		static constexpr bool value = is_list<T<TStorage>>::value;
	};

	/// @see is_list_type
	template<typename TStorage, size_t n, template<typename, size_t, typename ...> class T, typename ... Args>
	struct is_list_type<TStorage, T<TStorage, n, Args...>>
	{
		static constexpr bool value = is_list<T<TStorage, n>>::value;
	};

	/// @see is_vert
	template<typename T>
	constexpr bool is_vert_v = is_vert<T>::value;
	/// @see is_vertices
	template<typename T>
	constexpr bool is_vertices_v = is_vertices<T>::value;
	/// @see is_vertices_type
	template<typename TStorage, size_t d, typename T>
	constexpr bool is_vertices_type_v = is_vertices_type<TStorage, d, T>::value;
	/// @see is_vertices_vtype
	template<typename TStorage, typename T>
	constexpr bool is_vertices_vtype_v = is_vertices_vtype<TStorage, T>::value;
	/// @see is_list
	template<typename T>
	constexpr bool is_list_v = is_list<T>::value;
	/// @see is_list_type
	template<typename TStorage, typename T>
	constexpr bool is_list_type_v = is_list_type<TStorage, T>::value;

	// view class for verticies
	// TODO: this is never used, should be deleted?
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

	/// @brief ostream opeartor<< overload for arVertex
	/// outputs: "(vert.x, vert.y)" to the passed stream
	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertex<T, n>& vert);

	/// @brief ostream opeartor<< overload for arVertex
	/// outputs: "(vert1.x, vert1.y), (vert2.x, vert2.y), ..., (vertn.x, vertn.y)" to the passed stream
	template<typename T, size_t n>
	std::ostream& operator<<(std::ostream& stream, const arVertices<T, n>& verts);
}

#include "Vertices.ipp"
