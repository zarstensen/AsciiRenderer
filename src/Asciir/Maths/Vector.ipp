#include "Vector.h"
#include "Maths.h"

namespace Asciir
{
	template<typename T, size_t d>
	bool arVector<T, d>::perp(const arVector<T, d>& other)
	{
		return std::abs(this->dot(other)) == std::abs(this->norm() * other.norm());
	}
}