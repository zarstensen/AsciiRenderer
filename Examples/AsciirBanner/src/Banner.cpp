#include <Asciir.h>

#include <FastNoise/FastNoise.h>

using namespace Asciir;
using namespace Asciir::AsciirLiterals;

using namespace FastNoise;

class LightningShader : public Shader2D
{
	virtual TermVert size() const override
	{
		return { -1, -1 };
	}

	virtual Tile readTile(const TermVert& coord, Coord uv, const DeltaTime& time_since_start, size_t frames_since_start) override
	{
		Coord _uv = toUV(coord);

		Colour background;

		// the noise used is OpenSimplex 2, the closer the uv coords are to the edge of the screen, the closer to 0 the rng values will be.

		Real rng = std::min(1_R, 2*std::pow(std::sin(_uv.x * PI), 2_R)) * noise_generator->GenSingle2D(_uv.x, time_since_start, seed);

		// this generates a generate starting at the centre of the screen that expands horizontally
		Real scale = (1 - std::pow(std::tanh(_uv.y * 4 - 2 + 0.25 * rng), 2));

		// colour the gradient, and make sure it is white in the centre

		background.red = 255 * round(std::pow(scale, 2), 1 / 64_R);
		background.green = 255 * round(std::pow(scale, 25), 1 / 64_R);
		background.blue = 255 * round(std::pow(scale, 1), 1 / 64_R);
		background.alpha = 255;

		return background;
	}

	// OpenSimplex 2 noise
	SmartNode<> noise_generator = NewFromEncodedNodeTree("EwAAAIBADQAQAAAAj8IVQQkAAM3MTD4AAAAAAA==");
	int seed = 255;
};

// renders the passed shader with an outline.
class OutlineShader : public Shader2D
{
public:
	OutlineShader(Ref<Shader2D> shader, Tile outline)
		: m_shader(shader), m_outline(outline)
	{}

	// the size should be 1 "pixel" larger for each side, as there might need to be an outline tile there
	virtual TermVert size() const override
	{
		return m_shader->size() + TermVert(2, 2);
	}

	virtual Tile readTile(const TermVert& coord, Coord uv, const DeltaTime& time_since_start, size_t frames_since_start) override
	{
		TermVert shader_coord = coord - TermVert(1, 1);
		Quad shader_quad = Quad::fromCorners(Coord(1, 1), (Coord)m_shader->size());
		// check if any of the neighbouring tiles exists, if they do, and you are on an empty tile, fill it with the outline tile.

		if(!shader_quad.isInsideGrid(coord) || m_shader->readTile(shader_coord).background_colour.alpha == 0)
			for (TInt y = -1; y <= 1; y++) {
				for (TInt x = -1; x <= 1; x++)
				{
					// the outline should be drawn at coord if this is true
					if (shader_quad.isInsideGrid(coord + TermVert(x, y)) && m_shader->readTile(shader_coord + TermVert(x, y)).background_colour.alpha > 0)
						return m_outline;
				}
			}

		if (shader_quad.isInsideGrid(coord))
			return m_shader->readTile(shader_coord);
		else
			return Tile::emptyTile();
	}

protected:
	Ref<Shader2D> m_shader;
	Tile m_outline;
};

class BannerLayer : public Layer
{
public:
	virtual void onAdd() override
	{
		// setup all the shaders

		m_text.allocate();
		m_text->load("resources/Asciir.png");

		auto s = m_text->size();

		m_outlined_text = OutlineShader(m_text, Tile(BLACK8));

		m_lightning.allocate();
	};

	virtual void onUpdate(DeltaTime dt) override
	{
		// the text should be at the centre of the screen
		m_text_transform.setPos((Coord)Renderer::size() / 2_R - m_text->centre() - Coord(0, 1));

		Renderer::resize({ Renderer::maxSize().x, Renderer::maxSize().y / 2 });
		Renderer::clear();
		
		// the lightning shader is submitted before the text shader, so it will be rendered underneath the text.
		Renderer::submit(m_lightning);

		// as the text texture needs to be centred, a transform is submitted along with the shader to achieve this.
		Renderer::submit(m_outlined_text, m_text_transform);
		
		Renderer::setTitle(std::to_string(dt.fps()));
	}

protected:

	Ref<LightningShader> m_lightning;
	Ref<OutlineShader> m_outlined_text;
	Transform m_text_transform;
	Ref<FileTexture> m_text;
};

class AsciirBanner : public ARApp
{
public:
	virtual void start(const std::vector<std::string>& args) override
	{
		pushLayer(new BannerLayer());

		Renderer::setThreads();
		Renderer::setFont("Consolas", { 10, 10 });
	}
};

AR_DEFAULT_ENTRYPOINT(AsciirBanner)
