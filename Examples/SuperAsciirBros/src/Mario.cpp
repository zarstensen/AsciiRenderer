#include "Mario.h"

void Mario::render()
{
	if (m_velocity.x > 0)
	{
		m_transform.setScale({ 1, 1 });
	}
	else if (m_velocity.x < 0)
	{
		m_transform.setScale({ -1, 1 });
	}
	
	if (m_velocity.x != 0)
	{
		m_sprite.setSprite({ (m_sprite.getSpritePos().x + 1) % 3, 1 });
	}
	else
	{
		m_sprite.setSprite({ 0, 0 });
	}

	m_transform.setPos(m_position - Coord(0, m_sprite.textureSize().y));
	m_transform.setOrigin(m_sprite.centre());
	
	
	Renderer::submit(m_sprite.getSprite(), m_transform);
}

void Mario::update(DeltaTime dt)
{
	m_position += m_velocity * dt.seconds();

	m_velocity.y += GRAV * dt.seconds();

	TInt ground = Renderer::height() - Ground::GROUND_HEIGHT + 2;

	if (ground - m_position.y <= 0)
	{
		m_position.y = Renderer::height() - Ground::GROUND_HEIGHT + 2;
		m_velocity.y = 0;
	}

}
