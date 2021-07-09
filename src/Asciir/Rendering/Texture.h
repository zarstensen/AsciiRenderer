#pragma once

#include "Asciir/Math/Vertices.h"
#include "TerminalRenderer.h"

namespace Asciir
{
	// A class used to access texture data
	// Is inherited by FileTexture and DynamicTexture
	class Texture
	{
	public:
		virtual ~Texture() {};
		virtual Size2D size() const = 0;
		virtual Tile readTile(const Size2D& coord) const = 0;
	};
}
