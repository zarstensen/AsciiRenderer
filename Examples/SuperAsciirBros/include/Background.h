#pragma once

#include <Asciir.h>

using namespace Asciir;

// simply clears the renderer with the background colour, as well as controls the size of the game.
// maybe clouds later?
class Background : public Layer
{
public:
	// baby blue kinda colour.
	Colour background_colour = Colour(92, 148, 252);
	TermVert game_size;

	// the background layer will resize the terminal to be the passed game size.
	// as the background should be the first layer, this can be overridden by other layers.
	// (-1, -1) = no resize
	Background(TermVert game_size)
		: game_size(game_size) {}

	void onAdd() final override;

	void onUpdate(DeltaTime) final override;
};