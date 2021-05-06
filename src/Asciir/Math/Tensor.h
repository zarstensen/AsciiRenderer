#pragma once

#include "Matrix.h"

namespace Asciir
{
	// A wrapper for arMatrix that overloads acsess functions to accept 3 indexes instead of 2.
	// 
	// resize and operator() gets the element by using w*h as the first index and d as the second index.
	template<typename T>
	struct arTensor3D : arMatrix<T>
	{
		arTensor3D() = default;
		arTensor3D(size_t w, size_t h, size_t d);
		arTensor3D(Size3D size);

		template<typename TOther>
		arTensor3D(const Eigen::MatrixBase<TOther>& other);

		Size3D size() const;
		void resize(Size3D size);

		T& operator()(size_t x, size_t y, size_t z);
		T& operator()(Size3D index);
		T& operator()(Size2D index) = delete;

		T& get(Size3D index);
		T& get(size_t x, size_t y, size_t z);
		T& get(Size2D index) = delete;

	protected:
		size_t m_width;
	};
}

#include "Tensor.ipp"
