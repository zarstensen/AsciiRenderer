#pragma once

#include "Matrix.h"

namespace Asciir
{
	/// @brief A wrapper for arMatrix that overloads acsess functions to accept 3 indexes instead of 2.  
	/// 
	/// implementation details:  
	/// resize and operator() gets the element by using w*h as the first index and d as the second index.
	/// everything else functions as usual
	/// 
	template<typename T>
	struct arTensor3D : arMatrix<T>
	{
		/// @brief Default constructor
		arTensor3D() : m_width(1) {};
		/// @brief construct a tensor of size w, h, d
		/// @param w width of the tensor
		/// @param h height of the tensor
		/// @param d depth of the tensor 
		arTensor3D(size_t w, size_t h, size_t d);
		/// @brief construct a tensor from a Size3D argument
		/// @param size size of the tensor
		arTensor3D(Size3D size);

		/// @brief constructor for eigen generic expressions
		template<typename TOther>
		arTensor3D(const Eigen::MatrixBase<TOther>& other);

		/// @brief returns the width, height and depth of the tensor
		Size3D size() const;
		void resize(Size3D size) { arMatrix<T>::resize(size.x * size.y, size.z); m_width = size.x; }

		/// @brief cheaper version of resize, but does not retain the matrix data
		/// @param size new size of the matrix
		void resizeClear(Size3D size) { arMatrix<T>::resizeClear(size.x * size.y, size.z); m_width = size.x; }

		/// @brief access the element at the 3d coordinate passed
		T& operator()(size_t x, size_t y, size_t z);
		/// @brief access the element at the 3d coordinate passed
		T& operator()(Size3D index);
		T& operator()(Size2D index) = delete;

		/// @see operator()(size_t, size_t, size_t)
		T& get(size_t x, size_t y, size_t z);
		/// @see operator()(Size3D)
		T& get(Size3D index);
		T& get(Size2D index) = delete;

	protected:
		// store the width, as the matrix is constructed as (w * h, d), so the information about width and height is lost.
		size_t m_width;
	};
}

#include "Tensor.ipp"
