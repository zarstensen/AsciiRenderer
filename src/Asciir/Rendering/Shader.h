#pragma once

#include "Asciir/Maths/Vertices.h"
#include "TerminalRenderer.h"

namespace Asciir
{
	/// @brief A base class used to access dynamiccly generated data as a texture.  
	/// is computed on the CPU and not the GPU, so this is not actually a shader, but functions as one.  
	/// Should be inherited, if the application wants a structure to generate texture data at runtime.
	class Shader2D
	{
	public:
		virtual ~Shader2D() {};

		/// @brief get the size of the shader from which data can be read from.  
		virtual Size2D size() const = 0;

		/// @brief access the shader data from the shader
		/// @param coord the coordinate of the wanted tile
		/// @param time_since_start (optional) time value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		/// @param frames_since_start (optional) frame value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		virtual Tile readTile(const Size2D& coord, const DeltaTime& time_since_start = 0, const size_t& frames_since_start = 0) const = 0;
	};
}
