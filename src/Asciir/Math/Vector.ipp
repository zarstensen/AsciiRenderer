#include "Vector.h"

namespace Asciir
{
	template<typename T, size_t d>
	bool arVector<T, d>::perp(const arVector<T, d>& other)
	{
		return this->dot(other) == this->norm() * other.norm();
	}
}
