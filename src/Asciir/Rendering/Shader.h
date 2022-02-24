#pragma once

#include "Asciir/Math/Vertices.h"
#include "TerminalRenderer.h"

namespace Asciir
{
	// A base class used to access dynamiccly generated data as a texture. runs on the CPU and not GPU.  
	// Should be inherited, if the application wants a structure to generate texture data at runtime.
	class Shader2D
	{
	public:
		virtual ~Shader2D() {};

		virtual Size2D size() const = 0;

		/// @brief access the shader data from the shader
		/// @param coord the coordinate of the wanted tile
		/// @param time_since_start (optional) time value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		/// @param frames_since_start (optional) frame value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		virtual const Tile& readTile(const Size2D& coord, const DeltaTime& time_since_start = 0, const size_t& frames_since_start = 0) const = 0;
	};
}
