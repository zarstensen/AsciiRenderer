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
	arMatrix<T> arMatrix<T>::operator=(const arMatrix<T>& other)
	{
		return arMatrix<T>(other);
	}
	
	template<typename T>
	Size2D arMatrix<T>::size() const
	{

		size_t height = (size_t)Eigen::MatrixX<T>::cols();
		size_t width = (size_t)Eigen::MatrixX<T>::rows();

		AR_ASSERT_MSG(width >= 0 && height >= 0, "Invalid coords: ", width, ',', height);

		return { width, height };
	}
	
	template<typename T>
	void arMatrix<T>::resize(Size2D size)
	{
		Eigen::MatrixX<T>::conservativeResize(size.x, size.y);
	}

	template<typename T>
	T& arMatrix<T>::operator[](size_t indx)
	{
		return Eigen::MatrixX<T>::operator()(indx);
	}

	template<typename T>
	T& arMatrix<T>::get(Size2D coord)
	{

		AR_ASSERT_MSG(coord.x < size().x && coord.y < size().y, "Invalid coord: ", coord);

		return Eigen::MatrixX<T>::operator()(coord.x, coord.y);
	}

}
