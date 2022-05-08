#pragma once

#include <Asciir.h>

using namespace Asciir;

// represents a ground section of a scene
class Ground
{
public:
	static constexpr size_t GROUND_HEIGHT = 2; // TODO: 5 + fullscreen -> EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

	Ground(TInt position = 0, size_t width = 0);

	// renders the ground object to the frame.r
	void render();

protected:
	FileTexture m_ground_texture;
	Ref<Transform> m_ground_transform;
	TInt m_position;
	size_t m_width;
};
