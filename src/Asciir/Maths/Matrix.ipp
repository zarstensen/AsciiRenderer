#pragma once

#include "Matrix.h"
#include "Asciir/Logging/Log.h"

namespace Asciir
{
	template<typename T>
	template<typename TOther>
	arMatrix<T>::arMatrix(const arMatrix<TOther>& other)
		: Eigen::MatrixX<T>((Eigen::MatrixX<T>)other) {}

	template<typename T>
	template<typename TOther>
	arMatrix<T>::arMatrix(const Eigen::MatrixBase<TOther>& other)
		: Eigen::MatrixX<T>(other.template cast<T>()) {}

	template<typename T>
	arMatrix<T>::arMatrix(Size2D size)
		: Eigen::MatrixX<T>(size.x, size.y) {}

	template<typename T>
	Size2D arMatrix<T>::size() const
	{
		size_t width = (size_t)Eigen::MatrixX<T>::cols();
		size_t height = (size_t)Eigen::MatrixX<T>::rows();

		return { height, width };
	}

	template<typename T>
	void arMatrix<T>::resize(Size2D new_size)
	{
		Eigen::MatrixX<T>::conservativeResize(new_size.x, new_size.y);
	}

	template<typename T>
	void arMatrix<T>::resizeClear(Size2D size)
	{
		Eigen::MatrixX<T>::resize(size.x, size.y);
	}

	template<typename T>
	T& arMatrix<T>::operator[](size_t indx)
	{
		AR_ASSERT_MSG(indx < (size_t) Eigen::MatrixX<T>::size(), "Index out of range: ", indx);

		//return *(data() + indx);
		return Eigen::MatrixX<T>::operator()(indx);
	}

	template<typename T>
	const T& arMatrix<T>::operator[](size_t indx) const
	{
		AR_ASSERT_MSG(indx < Eigen::MatrixX<T>::size(), "Index out of range: ", indx);

		//return *(data() + indx);
		return Eigen::MatrixX<T>::operator()(indx);
	}

	template<typename T>
	T& arMatrix<T>::get(Size2D coord)
	{
		AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Invalid coord: ", coord);

		//return *(data() + coord.y * cols() + coord.x);
		return Eigen::MatrixX<T>::operator()(coord.x, coord.y);
	}

	template<typename T>
	const T& arMatrix<T>::get(Size2D coord) const
	{
		AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Invalid coord: ", coord);

		//return *(data() + coord.y * cols() + coord.x);
		return Eigen::MatrixX<T>::operator()(coord.x, coord.y);

	}

	template<typename T>
	T& arMatrix<T>::get(size_t x, size_t y)
	{
		AR_ASSERT_MSG(x < size().x&& y < size().y, "Invalid coord: ", x, ',', y);

		// return *(data() + y * cols() + x);
		return Eigen::MatrixX<T>::operator()(x, y);
	}

	template<typename T>
	const T& arMatrix<T>::get(size_t x, size_t y) const
	{
		AR_ASSERT_MSG(x < size().x&& y < size().y, "Invalid coord: ", x, ',', y);

		return Eigen::MatrixX<T>::operator()(x, y);
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
	Size2D s_arMatrix<T, w, h>::size()
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
		AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Invalid coord: ", coord);

		return Eigen::Matrix<T, w, h>::operator()(coord.x, coord.y);
	}

	template<typename T, size_t w, size_t h>
	T s_arMatrix<T, w, h>::get(Size2D coord) const
	{
		AR_ASSERT_MSG(coord.x < size().x&& coord.y < size().y, "Invalid coord: ", coord);

		return Eigen::Matrix<T, w, h>::operator()(coord.x, coord.y);
	}

	template<typename T, size_t w, size_t h>
	T& s_arMatrix<T, w, h>::get(size_t x, size_t y)
	{
		AR_ASSERT_MSG(x < size().x&& y < size().y, "Invalid coord: ", x, ',', y);

		return Eigen::Matrix<T, w, h>::operator()(x, y);
	}

	template<typename T, size_t w, size_t h>
	T s_arMatrix<T, w, h>::get(size_t x, size_t y) const
	{
		AR_ASSERT_MSG(x < size().x&& y < size().y, "Invalid coord: ", x, ',', y);

		return Eigen::Matrix<T, w, h>::operator()(x, y);
	}
}