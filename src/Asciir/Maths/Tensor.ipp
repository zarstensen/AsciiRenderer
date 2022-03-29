#include "Tensor.h"

namespace Asciir
{
	template<typename T>
	arTensor3D<T>::arTensor3D(size_t w, size_t h, size_t d)
		: arMatrix<T>(w * h, d), m_width(w) {}

	template<typename T>
	arTensor3D<T>::arTensor3D(Size3D size)
		: arTensor3D<T>(size.x, size.y, size.z) {}

	template<typename T>
	template<typename TOther>
	arTensor3D<T>::arTensor3D(const Eigen::MatrixBase<TOther>& other)
		: arMatrix<T>(other.template cast<T>()) {}

	template<typename T>
	Size3D arTensor3D<T>::dim() const
	{
		Size2D mat_size = arMatrix<T>::dim();

		return { m_width, mat_size.x / m_width, mat_size.y };
	}

	template<typename T>
	T& arTensor3D<T>::operator()(size_t x, size_t y, size_t z)
	{
		return get(x, y, z);
	}

	template<typename T>
	T& arTensor3D<T>::operator()(Size3D index)
	{
		return get(index);
	}

	template<typename T>
	T& arTensor3D<T>::get(Size3D index)
	{
		AR_ASSERT_MSG(index.x < size().x&& index.y < size().y&& index.z < size().z, "Index out of bounds: ", index, " bounds: ", size());

		return arMatrix<T>::operator()(index.x * size().y + index.y, index.z);
	}

	template<typename T>
	T& arTensor3D<T>::get(size_t x, size_t y, size_t z)
	{
		return arTensor3D<T>::operator()(Size3D(x, y, z));
	}
}