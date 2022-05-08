#include "Ground.h"

// load image file and set up attributes
Ground::Ground(TInt position, size_t width)
	: m_position(position), m_width(width)
{
	// m_ground_texture.load("resources/textures/Ground.png");
	m_ground_transform = new Transform;

	m_ground_transform->setPos({ position, Renderer::height() - m_ground_texture.textureSize().y * GROUND_HEIGHT});
	m_ground_texture.setTiledSize({ width * m_ground_texture.textureSize().x, m_ground_texture.textureSize().y * GROUND_HEIGHT});
	m_ground_transform->calcMat();
}

void Ground::render()
{
	m_ground_transform->setPos({ m_ground_transform->getPos().x, Renderer::height() - m_ground_texture.textureSize().y * GROUND_HEIGHT});

	Renderer::submit(m_ground_texture, *m_ground_transform);
}
