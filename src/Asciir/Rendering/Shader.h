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
		/// @return the maximum coord that can be put into readTile().
		/// (-1, -1) = no limit
		virtual TermVert size() const = 0;

		/// @brief access the shader data from the shader
		/// @param coord the coordinate of the wanted tile
		/// @param time_since_start (optional) time value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		/// @param frames_since_start (optional) frame value for the shader function. Is automaticly supplied if passed through Renderer::submitShader
		virtual Tile readTile(TermVert coord, const DeltaTime& time_since_start = 0, size_t frames_since_start = 0) = 0;
		
		/// @brief maps the given coordinate to a 0-1 range in the x and y dimension.
		/// 
		/// (0, 0) will be the top left of the shader and (1, 1) will be the bottom right of the shader
		///  
		/// if the size() is unlimited, the uv coordinates will be relative to the terminal size.
		/// 
		/// @param coord 
		/// @return 
		Coord toUV(const TermVert& coord);
	};
}
