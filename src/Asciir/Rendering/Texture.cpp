#include "Texture.h"
#include "arpch.h"

namespace Asciir
{
	// ============ Texture2D ============
	
	Texture2D::Texture2D(const Size2D& new_size, const Tile& new_tile)
		: m_texture(new_size)
	{
		m_texture.fill(new_tile);
	}

	// uv, dt, and df is never used here
	Tile Texture2D::readTile(const TermVert& coord, Coord, const DeltaTime&, size_t)
	{
		return m_texture(coord);
	}

	void Texture2D::setTile(const Size2D& coord, const Tile& new_tile)
	{
		m_texture(coord) = new_tile;
	}

	void Texture2D::blendTile(const Size2D& coord, const Tile& overlay_tile)
	{
		m_texture(coord).blend(overlay_tile);
	}

	TermVert Texture2D::size() const
	{
		return TermVert((TInt) m_texture.rows(), (TInt) m_texture.cols());
	}

	void Texture2D::resize(const Size2D& new_size, RESIZE mode, const Tile& fill_tile)
	{
		// call function depending on mode
		switch (mode)
		{
			case RESIZE::CLEAR:
				resizeClear(new_size, fill_tile);
				break;
			case RESIZE::FILL:
				resizeFill(new_size, fill_tile);
				break;
			case RESIZE::NEAREST:
				resizeNearest(new_size);
				break;
		}
	}

	void Texture2D::resizeClear(const Size2D& new_size, const Tile& fill_tile)
	{
		m_texture.resizeClear(new_size);

		// only fill texture with fill_tile if it is not the default tile value
		if (fill_tile != Tile())
			m_texture.fill(fill_tile);
	}

	void Texture2D::resizeFill(const Size2D& new_size, const Tile& fill_tile)
	{
		Size2D prev_size = size();
		m_texture.resize(new_size);

		if (fill_tile != Tile())
		{
			// fill newly resized part of texture with fill_tile
			if (new_size.y > prev_size.y)
			{
				// top to bottom should fill the most values, as the data should be stored continuously
				m_texture.block(prev_size.y, 0, new_size.y - prev_size.y, new_size.x).fill(fill_tile);
			}

			if (new_size.x > prev_size.x)
			{
				m_texture.block(0, prev_size.x, new_size.y, new_size.x - prev_size.x).fill(fill_tile);
			}
		}
	}

	// ============ FileTexture ============
	void FileTexture::load(const Path& dir)
	{
		Path abs_dir = std::filesystem::absolute(dir);
		AR_ASSERT_MSG(!m_is_loaded, "cannot load file into an already loaded texture");

		if (std::filesystem::exists(abs_dir))
		{
			m_file_dir = abs_dir;
			m_is_loaded = true;

			std::ifstream texture_in(abs_dir, std::ios::binary);

			// load the size into memory

			size_t width;
			size_t height;

			texture_in.read((char*)&width, sizeof(size_t));
			texture_in.read((char*)&height, sizeof(size_t));

			// allocate memory for the texture

			resize({width, height});

			// load texture into memory
			for (auto& elem : m_texture.reshaped())
			{
				Tile tile_in;
				texture_in.read((char*)tile_in.symbol, 1);
				texture_in.read((char*)tile_in.symbol + 1, U8CharSize(tile_in.symbol) - 1);

				texture_in.read((char*)&tile_in.colour, sizeof(elem.colour));

				texture_in.read((char*)&tile_in.background_colour, sizeof(tile_in.background_colour));

				elem = tile_in;
			}
		}
		else
		{
			AR_CORE_ERR("the file ", dir, " does not exist, unable to load texture");
			AR_ASSERT(false);
		}
	}

	void FileTexture::unload()
	{
		AR_ASSERT_MSG(m_is_loaded, "cannot unload already unloaded texture");
		m_is_loaded = false;

		resize({ 0, 0 });
	}

	void FileTexture::reload()
	{
		AR_ASSERT_MSG(m_is_loaded, "cannot reload unloaded texture");
		m_is_loaded = false;
		load(m_file_dir);
	}
}
