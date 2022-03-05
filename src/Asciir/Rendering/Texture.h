#pragma once

#include "Shader.h"

namespace Asciir
{

	enum class RESIZE
	{
		/// @brief clears all data stored in the texture
		CLEAR,
		/// @brief increases the size of the texture without doing anything to the existing data
		FILL,
		/// @brief upscales the stored texture using the nearest neighbour technique
		/// @attention **this mode is not yet implemented**
		NEAREST
	};

	/// @brief A simple shader storing a resizable 2D ascii texture
	class Texture2D : public Shader2D
		/// @brief Default constructor.  
	{
	public:
		/// simply constructs a texture with size (0, 0)
		Texture2D() {}

		/// @brief Constructs a texture of the passed size and fills it with the passed Tile
		/// @param new_size size of the texture
		/// @param new_tile Tile the texture should be filled with
		Texture2D(const Size2D& new_size, const Tile& new_tile = Tile());


		/// @brief read a tile from the texture
		/// @param coord the coordinate of the wanted tile
		/// @param uv *reserved*
		/// @param dt *reserved*
		/// @param df *reserved*
		Tile readTile(const Size2D& coord, Coord uv, const DeltaTime& dt = 0, size_t df = 0) const override;
		
		/// @return the size of the texture 
		Size2D size() const override;

		/// @brief resizes the size of the stored texture
		/// @param new_size the size the texture should be resized to
		/// @param mode the method of resizing
		/// @param fill_tile the tile value that should be used to fill any new tiles generated from the RESIZE::FILL or RESIZE::CLEAR modes
		/// @see resizeClear()
		/// @see resizeFill()
		/// @see resizeNearest
		void resize(const Size2D& new_size, RESIZE mode = RESIZE::FILL, const Tile& fill_tile = Tile());

		/// @brief resize function for the RESIZE::CLEAR mode
		void resizeClear(const Size2D& new_size, const Tile& fill_tile);
		/// @brief resize function for the RESIZE::FILL mode
		void resizeFill(const Size2D& new_size, const Tile& fill_tile);
		/// @brief resize function for the RESIZE::NEAREST mode
		void resizeNearest(const Size2D& new_size) {};

		/// @brief sets tile values inside the texture
		/// @param coord the coordinate of the target tile in the texture
		/// @param new_tile the value of the tile to be placed at the coords
		void setTile(const Size2D& coord, const Tile& new_tile);

		/// @brief blends new tiles onto the texture
		/// 
		/// if overlay_tile has an opacity of 100% this has the same fucntionality as setTile(), although is still more expensive to call.
		/// @param coord the coordinate of the target tile in the texture
		/// @param overlay_tile the tile to be blended onto the target tile.
		void blendTile(const Size2D& coord, const Tile& overlay_tile);

	protected:
		arMatrix<Tile> m_texture;
	};

	typedef std::filesystem::path Path;

	// stores data from a .cart (compact asciir Texture) file
	class FileTexture : public Texture2D
	{
	public:
		FileTexture() = default;
		FileTexture(const Path& file_dir) { load(file_dir); };

		~FileTexture() final override { if (loaded()) unload(); }

		void load(const Path& dir);
		void unload();

		void reload();

		bool loaded() const { return m_is_loaded; }

		Path dir() const { return m_file_dir; };

	protected:
		Path m_file_dir;

		bool m_is_loaded = false;
	};
}
