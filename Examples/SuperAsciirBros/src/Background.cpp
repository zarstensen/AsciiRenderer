#include "Background.h"

void Background::onAdd()
{
}

void Background::onUpdate(DeltaTime dt)
{
	Tile clear_tile;


	// Renderer::setFontSize({ 8, 8 });

	clear_tile.background_colour = background_colour;
	clear_tile.colour = background_colour;

	Renderer::clear(clear_tile);

	/*if (game_size != TermVert(-1, -1))
		Renderer::resize(Renderer::maxSize().unaryExpr([](const size_t val) { return val - 40; }));*/

	// Resize should be a request.
	Renderer::resize(Renderer::maxSize());

	Renderer::setTitle(std::to_string(dt.fps()));
}

