#pragma once

#include "Vertices.h"

namespace Asciir
{
	/// @brief A wrapper for MatrixX that overloads functions like size() to return Asciir types instead of Eigen types.
	/// @tparam T datatype stored in the matrix
	template<typename T>
	struct arMatrix : public Eigen::MatrixX<T>
	{
		using Eigen::MatrixX<T>::MatrixX;

		/// @brief Constructor taking a size parameter defining the size of the dynamic array
		arMatrix(Size2D size);

		/// @brief copy constructor
		template<typename TOther>
		arMatrix(const arMatrix<TOther>& other);


		/// @brief constructor for eigen generic expressions
		template<typename TOther>
		arMatrix(const Eigen::MatrixBase<TOther>& other);

		/// @brief returns the 2D size of the array (the dimentions)
		Size2D dim() const;

		/// @brief resize the matrix whilst preserving the data contained.
		/// see resizeClear() for a faster but more destructive resize.
		void resize(Size2D size);

		/// @brief cheaper version of resize, but does not retain the matrix data
		/// @param size new size of the matrix
		void resizeClear(Size2D size);

		/// @brief access the n'th element in the flattened matrix
		/// @return reference to the element at indx
		T& operator[](size_t indx);
		/// @brief const version of operator[]()
		/// @see operator[]()
		const T& operator[](size_t indx) const;
		/// @brief access the element at the passed coordinate.
		T& operator()(Size2D coord) { return get(coord); }
		/// @brief const version of operator()()
		/// @see operator()()
		const T& operator()(Size2D coord) const { return get(coord); }
		/// @brief same as, operator()(Size2D)
		T& operator()(size_t x, size_t y) { return get(x, y); }
		/// @brief same as, operator()(Size2D) const
		const T& operator()(size_t x, size_t y) const { return get(x, y); }

		/// @see operator()(Size2D)
		T& get(Size2D coord);
		/// @see operator()(Size2D) const
		const T& get(Size2D coord) const;
		/// @see operator()(size_t, size_t)
		T& get(size_t x, size_t y);
		/// @see operator()(size_t, size_t) const
		const T& get(size_t x, size_t y) const;
	};

	/// @brief A wrapper for a static Eigen matrix
	/// @tparam T datatype stored in the matrix
	/// @tparam w the width of the static matrix
	/// @tparam h the height of the static matrix
	template<typename T, size_t w, size_t h>
	struct s_arMatrix : public Eigen::MatrixX<T>
	{
		using Eigen::Matrix<T, w, h>::MatrixX;

		/// @brief copy constructor
		template<typename TOther>
		s_arMatrix(const s_arMatrix<TOther, w, h>& other);


		/// @brief constructor for eigen generic expressions
		template<typename TOther>
		s_arMatrix(const Eigen::EigenBase<TOther>& other);
		/// @brief constructor for eigen generic expressions
		template<typename TOther>
		s_arMatrix(const Eigen::ReturnByValue<TOther>& other);

		/// @see arMatrix::size()
		static Size2D dim();

		s_arMatrix<T, w, h> operator=(const s_arMatrix<T, w, h>& other);

		/// @see arMatrix::operator[](size_t)
		T& operator[](size_t indx);
		/// @see arMatrix::operator[](size_t) const
		T operator[](size_t indx) const;
		/// @see arMatrix::operator[](Size2D)
		T& operator()(Size2D coord) { return get(coord); }
		/// @see arMatrix::operator[](Size2D) const
		T operator()(Size2D coord) const { return get(coord); }
		/// @see arMatrix::operator[](size_t, size_t)
		T& operator()(size_t x, size_t y) { return get(x, y); }
		/// @see arMatrix::operator[](size_t, size_t) const
		T operator()(size_t x, size_t y) const { return get(x, y); }

		/// @see arMatrix::get(Size2D)
		T& get(Size2D coord);
		/// @see arMatrix::get(Size2D) const
		T get(Size2D coord) const;
		/// @see arMatrix::get(size_t, size_t)
		T& get(size_t x, size_t y);
		/// @see arMatrix::get(size_t, size_t) const
		T get(size_t x, size_t y) const;
	};

	// typedef / using / variables for Eigen::Matrix
	// s_ = static size

	template<typename T>
	static const arMatrix<T> arMatrix2 = arMatrix<T>(2, 2);

	template<typename T>
	static const arMatrix<T> arMatrix3 = arMatrix<T>(3, 3);

	template<typename T>
	static const arMatrix<T> arMatrix4 = arMatrix<T>(4, 4);

	template<typename T>
	using s_arMatrix2 = s_arMatrix<T, 2, 2>;

	template<typename T>
	using s_arMatrix3 = s_arMatrix<T, 3, 3>;

	template<typename T>
	using s_arMatrix4 = s_arMatrix<T, 4, 4>;
}

#include "Matrix.ipp"
