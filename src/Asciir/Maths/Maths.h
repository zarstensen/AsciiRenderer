#pragma once

#include "Asciir/Core/Core.h"
#include "Vertices.h"

namespace Asciir
{
	const extern Real PI;

	constexpr Real degToRad(Real degrees)
	{
		return degrees * PI / (Real)180;
	}

	constexpr Real radToDeg(Real radians)
	{
		return radians * (Real)180 / PI;
	}

	// TODO: find a better method of doing this
	/// @brief compare floats with a margin of error
	bool fcompare(Real a, Real b, Real margin = 0);


	/// @brief round the given real to the nearest value divisible by base.  
	///	
	/// the base represents the interval of the nearest value
	/// 
	/// for example:
	/// > round 2.5, base 1:  
	/// > 3  
	/// >  
	/// > round 2.5, base 2:  
	/// > 2  
	/// >  
	/// > round 2.5 base 0.5:  
	/// > 2.5  
	/// 
	/// @param val the value to be rounded
	/// @param base the base to be rounded in
	Real round(Real val, Real base = 1);
	/// @brief rounds both x, and y value using the given base
	/// see round(Real, Real) for implementation details
	Coord round(const Coord& coord, Real base = 1);
	/// @brief rounds the given value up to the nearest value divisible by base
	/// see round(Real, Real) for implementation details
	Real ceil(Real val, Real base = 1);
	/// @brief rounds the given Coord up to the nearest value divisible by base
	/// see round(Real, Real) for implementation details
	Coord ceil(const Coord& coord, Real base = 1);
	/// @brief rounds the given value down to the nearest value divisible by base
	/// see round(Real, Real) for implementation details
	Real floor(Real val, Real base = 1);
	/// @brief rounds the given Coord down to the nearest value divisible by base
	/// see round(Real, Real) for implementation details
	Coord floor(const Coord& coord, Real base = 1);
}
