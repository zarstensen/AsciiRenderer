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

		/// @brief copy constructor
		Texture2D(const Texture2D& other)
			: m_texture(other.m_texture), m_tiled_size(other.m_tiled_size) {}

		/// @brief move constructor
		Texture2D(Texture2D&& other) noexcept
			: m_texture(std::move(other.m_texture)), m_tiled_size(std::move(other.m_tiled_size)) {}

		/// @brief read a tile from the texture. if the coordinate is out of bounds, the texture will be tiled.
		/// @param coord the coordinate of the wanted tile
		/// @param uv *reserved*
		/// @param dt *reserved*
		/// @param df *reserved*
		Tile readTile(const TermVert& coord, Coord uv, const DeltaTime& dt = 0, size_t df = 0) override;
		
		/// @return the size of the texture, including tiling.
		TermVert size() const override;

		/// @brief sets the size of the texture, when tiled.
		/// if this new_size is greater than size(), the texture will be tiled, when using readTile().
		/// if set to (-1, -1), the tiled size will match the texture size.
		void setTiledSize(TermVert new_size) { m_tiled_size = new_size; }
		/// @brief gets the size of the stored texture.
		Size2D textureSize() const { return m_texture.dim(); }

		/// @brief retrieves the centre of the *tiled* texture
		Coord centre() { return (Coord) size() / 2; }

		/// @brief retrieves the centre of the *untiled* texture
		Coord textureCentre() { return (Coord) textureSize() / 2; }

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
		void resizeNearest(const Size2D& new_size);


		/// @brief sets tile values inside the texture. If the coord is out of bounds for the texture size, the coordinate will be mapped back to the texture, as if it was tiled.
		/// @param coord the coordinate of the target tile in the texture
		/// @param new_tile the value of the tile to be placed at the coords
		void setTile(const Size2D& coord, const Tile& new_tile);

		/// @brief blends new tiles onto the texture.
		/// Out of bounds: same as setTile().
		/// if overlay_tile has an opacity of 100% this has the same fucntionality as setTile(), although is still more expensive to call.
		/// @param coord the coordinate of the target tile in the texture
		/// @param overlay_tile the tile to be blended onto the target tile.
		void blendTile(const Size2D& coord, const Tile& overlay_tile);

		/// @brief move assignment operator.
		Texture2D& operator=(Texture2D&& other) noexcept
		{
			m_texture = std::move(other.m_texture);
			m_tiled_size = std::move(other.m_tiled_size);
			return *this;
		}

		/// @brief copy assignment operator.
		Texture2D& operator=(const Texture2D& other)
		{
			m_texture = other.m_texture;
			m_tiled_size = other.m_tiled_size;

			return *this;

		}

	protected:
		arMatrix<Tile> m_texture;
		TermVert m_tiled_size = {-1, -1};
	};

	typedef std::filesystem::path Path;

	/// @brief loads the given image file into a Texture2D class.
	/// 
	/// the image is loaded by writing the image RGB(A) values into the texture object.
	/// 
	/// @param image_path the path to the image that should be loaded
	/// @param use_half_tiles if set to true, each tile in the Texture2D will contain a half square symbol.
	/// each tile will then hold 2 colours, where the foreground colour and background colour each represents two seperate colours of the loaded image, on the y-axis.
	/// this works best with images with have an even y-resolution, in the case of an odd y-resolution, the bottom tiles will have a transparent foreground colour.
	/// if set to true, load_foreground is ignored.
	/// @return a Texture2D instance containing the image data
	/// @param load_foreground wether to load the image into the foreground or background colour.
	///	if set to false, the image is loaded into the background.
	Texture2D loadImage(Path image_path, bool use_half_tiles = false, bool load_foreground = false);

	/// @brief stores data from a .cart (compact asciir Texture) file
	class FileTexture : public Texture2D
	{
	public:
		FileTexture() = default;
		/// @brief autoloads the passed file
		FileTexture(const Path& file_dir) { load(file_dir); };

		~FileTexture() final override { if (loaded()) unload(); }

		/// @brief load the passed file path into memory
		void load(const Path& dir);
		/// @brief unload the texture and free the previosly loaded memory
		void unload();

		/// @brief reread the file texture into memory
		void reload();

		/// @brief returns wether the FileTexture currently has a texture loaded
		bool loaded() const { return m_is_loaded; }

		/// @brief the currently loaded texture file.  
		/// returns an empty path, if no file is currently loaded.
		Path dir() const { return m_file_dir; };

	protected:
		Path m_file_dir = "";

		bool m_is_loaded = false;
	};

	/// @brief stores a sequence of Textures.
	/// This class intended use is to store animations and to easily iterate through them.
	/// use nextFrame(), prevFrame() and setFrame(), to set what Texture2D readTile will use.
	/// 
	/// the active frame is automaticly preserved whenever the frame container is modified (addFrame, removeFrame, etc...), so a call to any of these functions will not change the active frame.
	/// the only case where the active frame is changed, is if the current active frame is removed.
	/// in this case, the next valid frame to the left of the active frame, will be chosen as the next active frame.
	/// if there are no valid frames to the left of the active frame, then the next valid frame to the right of the active frame will be chosen.
	/// 
	class TextureSequence
	{
	public:
		/// @brief default constructor, will contain 0 frames.
		TextureSequence() = default;

		/// @brief initializes the TextureSequence with the passed values
		TextureSequence(std::initializer_list<Ref<Texture2D>> init_values);

		TextureSequence(const TextureSequence& other);

		/// @brief increases the current frame index by a specified amount.
		/// 
		/// if the final frame index is greater than the number of frames, then the frame index is wrapped around to the beginning of the TextureSequence.
		/// 
		/// @param jump_size the size of the increment
		/// @return refrence to the Texture2D at the destination position
		Ref<Texture2D> incrFrame(size_t jump_size = 1);

		/// @brief decreases the current frame index by a specified amount.
		/// 
		/// if the final frame index is greater than the number of frames, then the frame index is wrapped around to the end of the TextureSequence.
		/// 
		/// @param jump_size the size of the increment
		/// @return refrence to the Texture2D at the destination position
		Ref<Texture2D> decrFrame(size_t jump_size = 1);
		/// @brief sets the index of the active frame
		/// @return refrence to the Texture2D at the passed frame index
		Ref<Texture2D> setFrame(size_t frame) { AR_ASSERT(frame < m_frames.size()); m_curr_frame = frame; return m_frames[m_curr_frame]; }

		/// @brief gets the active frame
		Ref<Texture2D> getFrame() const { return m_frames[m_curr_frame]; }
		/// @brief gets the frame at the passed frame index
		Ref<Texture2D> getFrame(size_t frame) const { return m_frames[frame]; }

		/// @brief gets the number of frames in the current TextureSequence
		size_t frameCount() { return m_frames.size(); }

		/// @brief adds a Texture2D to the end of the texture sequence
		void addFrame(Ref<Texture2D> new_frame) { m_frames.push_back(new_frame); }
		/// @brief inserts a Texture2D at the specified position
		void addFrame(Ref<Texture2D> new_frame, size_t pos);

		/// @brief appends the passed frames to the end of the TextureSequence
		void addFrames(const std::vector<Ref<Texture2D>>& init_values);
		/// @brief inserts the passed frames at the specified position
		void addFrames(const std::vector<Ref<Texture2D>>& init_values, size_t pos);
		/// @brief same as @ref addFrames(other.data())
		void addFrames(const TextureSequence& other) { addFrames(other.data()); }
		/// @brief same as @ref addFrames(other.data(), pos)
		void addFrames(const TextureSequence& other, size_t pos) { addFrames(other.data(), pos); }

		/// @brief removes the frame at the specified position.
		/// if the current active frame is removed, then the frame to the left of it will be the next active frame
		void removeFrame(size_t pos);
		/// @brief removes the frames inside the specified range.
		/// if the current active frame is removed, then the nearest left frame, relative to the current frame, will be chosen as the next active frame.
		void removeFrames(size_t start, size_t end);

		/// @brief removes all of the stored frames and resets the active frame index
		void clear() { m_frames.clear(); m_curr_frame = 0; }

		/// @brief reserve the specified amount of frames in the TextureSequence
		void reserve(size_t capacity) { m_frames.reserve(capacity); }

		/// @brief returns the vector instance storing the frames.
		const std::vector<Ref<Texture2D>>& data() const { return m_frames; }

		/// @brief same as getFrame()
		operator Ref<Texture2D>() { return getFrame(); }

	protected:
		size_t m_curr_frame = 0;
		std::vector<Ref<Texture2D>> m_frames;
	};
}
