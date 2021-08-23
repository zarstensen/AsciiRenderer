#pragma once

#include "Asciir/Math/Vertices.h"
#include "TerminalRenderer.h"

namespace Asciir
{
	// A base class used to access dynamiccly generated data as a texture. runs on the CPU and not GPU.
	// Should be inherited if the program wants to generate texture data at runtime
	// Is inherited by FileTexture
	class Shader2D
	{
	public:
		virtual ~Shader2D() {};

		virtual Size2D size() const = 0;
		virtual Tile readTile(const Size2D& coord, const DeltaTime& time_since_start, const size_t& frames_since_start) const = 0;
	};
}
