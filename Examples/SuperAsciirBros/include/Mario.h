#include <Asciir.h>

#include "Ground.h"

using namespace Asciir;
using namespace AsciirLiterals;

class Mario
{
public:

	Mario(Coord position = { 0, 0 })
		: m_position(position),
		m_sprite("resources/textures/SmallMarioSpriteSheet.png", { 16, 16 })
	{
		m_sprite.load();
	}

	void render();

	void update(DeltaTime dt);

	// sets the velocity vector of mario.
	void move(Real velocity)
	{
		m_velocity.x = velocity;
	}

	void jump(Real velocity)
	{
		m_velocity.y -= velocity;
	}

	void smallMario() {
		m_sprite.unload();

		m_sprite.load("resources/textures/SMBSmallMario.cart");
	}
	void largeMario() {
		m_sprite.unload();

		m_sprite.load("resources/textures/SMBSmallMario.cart");

	}
	void flowerMario() {
		m_sprite.unload();

		m_sprite.load("resources/textures/SMBSmallMario.cart");
	}

	Coord position() { return m_position; }

	SpriteSheet m_sprite;
protected:
	Coord m_position;
	Coord m_velocity;

	Transform m_transform;

	Real max_vel = 0.25_R;

	// ture -> right, false -> left
	bool direction = true;

	// super hacky solution until physics are a thing.
	static constexpr Real GRAV = 50_R;
};
