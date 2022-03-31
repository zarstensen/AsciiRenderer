#pragma once

#include "Matrix.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	template<typename T, int order>
	template<typename TOther, int order_other>
	arMatrix<T, order>::arMatrix(const arMatrix<TOther, order_other>& other)
		: EgMatrixX<T, order>((Eigen::MatrixX<T>)other) {}

	template<typename T, int order>
	template<typename TOther>
	arMatrix<T, order>::arMatrix(const Eigen::MatrixBase<TOther>& other)
		: EgMatrixX<T, order>(other.template cast<T>()) {}

	template<typename T, int order>
	arMatrix<T, order>::arMatrix(Size2D size)
		: EgMatrixX<T, order>(size.y, size.x) {}

	template<typename T, int order>
	Size2D arMatrix<T, order>::dim() const
	{
		size_t width = (size_t)EgMatrixX<T, order>::cols();
		size_t height = (size_t)EgMatrixX<T, order>::rows();

		return { width, height };
	}

	template<typename T, int order>
	void arMatrix<T, order>::resize(size_t h, size_t w)
	{
		EgMatrixX<T, order>::conservativeResize(h, w);
	}

	template<typename T, int order>
	void arMatrix<T, order>::resizeClear(size_t h, size_t w)
	{
		EgMatrixX<T, order>::resize(h, w);
	}

	template<typename T, int order>
	T& arMatrix<T, order>::operator[](size_t indx)
	{
		AR_ASSERT_MSG((indx < (size_t)EgMatrixX<T, order>::size()), "Index out of range: ", indx);

		//return *(data() + indx);
		return EgMatrixX<T, order>::operator()(indx);
	}

	template<typename T, int order>
	const T& arMatrix<T, order>::operator[](size_t indx) const
	{
		AR_ASSERT_MSG((indx < EgMatrixX<T, order>::size()), "Index out of range: ", indx);

		//return *(data() + indx);
		return EgMatrixX<T, order>::operator()(indx);
	}

	template<typename T, int order>
	T& arMatrix<T, order>::get(Size2D coord)
	{
		AR_ASSERT_MSG(coord.x < dim().x && coord.y < dim().y, "Invalid coord: ", coord);

		//return *(data() + coord.y * cols() + coord.x);
		return EgMatrixX<T, order>::operator()(coord.x, coord.y);
	}

	template<typename T, int order>
	const T& arMatrix<T, order>::get(Size2D coord) const
	{
		AR_ASSERT_MSG(coord.x < dim().x && coord.y < dim().y, "Invalid coord: ", coord);

		//return *(data() + coord.y * cols() + coord.x);
		return EgMatrixX<T, order>::operator()(coord.x, coord.y);

	}

	template<typename T, int order>
	T& arMatrix<T, order>::get(size_t y, size_t x)
	{
		AR_ASSERT_MSG(x < width() && y < height(), "Invalid coord: ", x, ',', y);

		// return *(data() + y * cols() + x);
		return EgMatrixX<T, order>::operator()(y, x);
	}

	template<typename T, int order>
	const T& arMatrix<T, order>::get(size_t y, size_t x) const
	{
		AR_ASSERT_MSG(x < width() && y < height(), "Invalid coord: ", x, ',', y);

		return EgMatrixX<T, order>::operator()(y, x);
	}

	template<typename T, size_t w, size_t h>
	template<typename TOther>
	s_arMatrix<T, w, h>::s_arMatrix(const s_arMatrix<TOther, w, h>& other)
		: Eigen::Matrix<T, w, h>((Eigen::Matrix<T, w, h>)other) {}

	template<typename T, size_t w, size_t h>
	template<typename TOther>
	s_arMatrix<T, w, h>::s_arMatrix(const Eigen::EigenBase<TOther>& other)
		: Eigen::Matrix<T, w, h>(other) {}

	template<typename T, size_t w, size_t h>
	template<typename TOther>
	s_arMatrix<T, w, h>::s_arMatrix(const Eigen::ReturnByValue<TOther>& other)
		: Eigen::Matrix<T, w, h>(other) {}

	template<typename T, size_t w, size_t h>
	s_arMatrix<T, w, h> s_arMatrix<T, w, h>::operator=(const s_arMatrix<T, w, h>& other)
	{
		return s_arMatrix<T, w, h>(other);
	}

	template<typename T, size_t w, size_t h>
	Size2D s_arMatrix<T, w, h>::dim()
	{
		return { w, h };
	}

	template<typename T, size_t w, size_t h>
	T& s_arMatrix<T, w, h>::operator[](size_t indx)
	{
		return Eigen::Matrix<T, w, h>::operator()(indx);
	}

	template<typename T, size_t w, size_t h>
	T s_arMatrix<T, w, h>::operator[](size_t indx) const
	{
		return Eigen::Matrix<T, w, h>::operator()(indx);
	}

	template<typename T, size_t w, size_t h>
	T& s_arMatrix<T, w, h>::get(Size2D coord)
	{
		AR_ASSERT_MSG(coord.x < w && coord.y < h, "Invalid coord: ", coord);

		return Eigen::Matrix<T, w, h>::operator()(coord.x, coord.y);
	}

	template<typename T, size_t w, size_t h>
	T s_arMatrix<T, w, h>::get(Size2D coord) const
	{
		AR_ASSERT_MSG(coord.x < w && coord.y < h, "Invalid coord: ", coord);

		return Eigen::Matrix<T, w, h>::operator()(coord.x, coord.y);
	}

	template<typename T, size_t w, size_t h>
	T& s_arMatrix<T, w, h>::get(size_t y, size_t x)
	{
		AR_ASSERT_MSG(x <w && y < h, "Invalid coord: ", x, ',', y);

		return Eigen::Matrix<T, w, h>::operator()(y, x);
	}

	template<typename T, size_t w, size_t h>
	T s_arMatrix<T, w, h>::get(size_t y, size_t x) const
	{
		AR_ASSERT_MSG(x < w && y < h, "Invalid coord: ", x, ',', y);

		return Eigen::Matrix<T, w, h>::operator()(y, x);
	}
}