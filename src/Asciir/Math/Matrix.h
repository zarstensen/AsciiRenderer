#pragma once

#include "Vertices.h"

namespace Asciir
{

	// A wrapper for MatrixX that overloads functions like size() to return Asciir types instead of Eigen types.
	template<typename T>
	struct arMatrix : public Eigen::MatrixX<T>
	{

		using Eigen::MatrixX<T>::MatrixX;
		
		template<typename TOther>
		arMatrix(const arMatrix<TOther>& other);


		Size2D size() const;

		void resize(Size2D size);

		arMatrix<T> operator=(const arMatrix<T>& other);
		
		T& operator[](size_t indx);

		T& get(Size2D coord);
	};

	// typedef / using / variables for Eigen::Matrix
	// s_ = static size

	template<typename T>
	constexpr arMatrix<T> arMatrix2 = arMatrix<T>(2, 2);

	template<typename T>
	constexpr arMatrix<T> arMatrix3 = arMatrix<T>(3, 3);

	template<typename T>
	constexpr arMatrix<T> arMatrix4 = arMatrix<T>(4, 4);

	template<typename T, size_t w, size_t h>
	using s_arMatrix = Eigen::Matrix<T, w, h>;

	template<typename T>
	using s_arMatrix2 = s_arMatrix<T, 2, 2>;

	template<typename T>
	using s_arMatrix3 = s_arMatrix<T, 3, 3>;

	template<typename T>
	using s_arMatrix4 = s_arMatrix<T, 4, 4>;
}

#include "Matrix.ipp"
