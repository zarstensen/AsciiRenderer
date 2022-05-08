#pragma once

#include <Asciir.h>

#include "Ground.h"
#include "Mario.h"

using namespace Asciir;

class Sun: public Shader2D
{
public:
	// Inherited via Shader2D
	virtual TermVert size() const override
	{
		return { -1, -1 };
	}

	virtual Tile readTile(const TermVert& coord, Coord uv = Coord(0, 0), const DeltaTime& time_since_start = 0, size_t frames_since_start = 0) override
	{
		Coord centre = Coord(10, 10);

		centre.x += time_since_start.seconds();

		Real radius = time_since_start.seconds() / 2;

		Coord offset = (Coord)coord - centre;

		if (std::sqrt(offset.x * offset.x + offset.y * offset.y) < radius)
		{
			Colour sun_c = YELLOW8;
			sun_c.alpha = 100;
			return Tile(sun_c, Colour(rand(), rand(), rand()), char(rand() % (94) + 32));
		}

		return Colour(0, 0);
	}
};

class GameLayer : public Layer
{
public:
	Ground ground;
	Mario mario;
	AudioSource mario_theme;
	AudioPlayer theme_player;

	Sun sun;

	// Renderer::height() - Ground::GROUND_HEIGHT

	GameLayer() {}

	void onAdd() final override
	{
		ground = Ground(0, Renderer::width());
		mario = Mario({ 0, 1 });

		mario_theme.setFile("resources/music/SMBTHEME.flac");
		
		theme_player.load(mario_theme);
		theme_player.getSrc<sf::Music>().setLoop(true);

		// theme_player.play();
	}

	void onUpdate(DeltaTime dt) final override
	{

		Real horz_vel = 0;
		Real speed = 35;
		 
		if (Input::isKeyDown(Key::D))
			horz_vel = speed;
		else if (Input::isKeyDown(Key::A))
			horz_vel = -speed;

		if (Input::isKeyDown(Key::W) && mario.position().y == Renderer::height() - Ground::GROUND_HEIGHT + 2)
			mario.jump(25);

		if (Input::isKeyDown(Key::LEFT_SHIFT))
			horz_vel *= 1.5;

		/*if (horz_vel)
			mario.m_sprite.incrSprite();
		else
			mario.m_sprite.setSprite({ 0, 0 });*/

		/*if (Input::isKeyToggled(Key::_1))
			mario.smallMario();
		else if (Input::isKeyToggled(Key::_2))
			mario.largeMario();
		else if (Input::isKeyToggled(Key::_3))
			mario.flowerMario();*/

		if (Input::isKeyDown(Key::F))
			Renderer::setTitle(std::to_string(dt.fps()));
		else if (Input::isKeyUntoggled(Key::F))
			Renderer::setTitle("Super Mario!");
		
		auto s = Renderer::size();


		mario.move(horz_vel);
		mario.update(dt);
		ground.render();
		mario.render();

		Renderer::submit(sun);
	}
};
