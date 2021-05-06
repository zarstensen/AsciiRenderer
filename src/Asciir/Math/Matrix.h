#pragma once

#include "Vertices.h"

namespace Asciir
{

	// A wrapper for MatrixX that overloads functions like size() to return Asciir types instead of Eigen types.
	template<typename T>
	struct arMatrix : public Eigen::MatrixX<T>
	{

		using Eigen::MatrixX<T>::MatrixX;
		
		arMatrix(Size2D size);

		template<typename TOther>
		arMatrix(const arMatrix<TOther>& other);

		// constructor for eigen generic expressions

		template<typename TOther>
		arMatrix(const Eigen::MatrixBase<TOther>& other);

		Size2D size() const;

		void resize(Size2D size);

		arMatrix<T> operator=(const arMatrix<T>& other);
		
		T& operator[](size_t indx);

		T& get(Size2D coord);
	};

	template<typename T, size_t w, size_t h>
	struct s_arMatrix : public Eigen::MatrixX<T>
	{

		using Eigen::Matrix<T, w, h>::MatrixX;

		template<typename TOther>
		s_arMatrix(const s_arMatrix<TOther, w, h>& other);

		// constructor for eigen generic expressions

		template<typename TOther>
		s_arMatrix(const Eigen::EigenBase<TOther>& other);
		template<typename TOther>
		s_arMatrix(const Eigen::ReturnByValue<TOther>& other);

		static Size2D size();

		s_arMatrix<T, w, h> operator=(const s_arMatrix<T, w, h>& other);

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

	template<typename T>
	using s_arMatrix2 = s_arMatrix<T, 2, 2>;

	template<typename T>
	using s_arMatrix3 = s_arMatrix<T, 3, 3>;

	template<typename T>
	using s_arMatrix4 = s_arMatrix<T, 4, 4>;
}

#include "Matrix.ipp"
