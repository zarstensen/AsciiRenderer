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
		/// if the new_size is greater than size(), the texture will be tiled, when using readTile().
		/// if the new_size is smaller than size(), the texture will be cropped.
		/// if set to (-1, -1), the tiled size will match the texture size.
		void setTiledSize(TermVert new_size) { m_tiled_size = new_size; }
		/// @brief gets the size of the stored texture.
		TermVert textureSize() const { return m_texture.dim(); }

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

	static constexpr std::array<uint32_t, 0x100> XP_FONT_MAP = {
	160,
	9786,
	9787,
	9829,
	9830,
	9827,
	9824,
	8226,
	9691,
	9675,
	9689,
	9794,
	9792,
	9834,
	9835,
	9788,
	9658,
	9668,
	8597,
	8252,
	182,
	167,
	9644,
	8616,
	8593,
	8595,
	8594,
	8592,
	8735,
	8596,
	9650,
	9660,
	160,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	8962,
	199,
	252,
	233,
	226,
	228,
	224,
	229,
	231,
	234,
	235,
	232,
	239,
	238,
	236,
	196,
	197,
	201,
	230,
	198,
	244,
	246,
	242,
	251,
	249,
	255,
	214,
	220,
	162,
	163,
	165,
	8359,
	402,
	225,
	237,
	243,
	250,
	241,
	209,
	170,
	186,
	191,
	8976,
	172,
	189,
	188,
	161,
	171,
	187,
	9617,
	9618,
	9619,
	9474,
	9508,
	9569,
	9570,
	9558,
	9557,
	9571,
	9553,
	9559,
	9565,
	9564,
	9563,
	9488,
	9492,
	9524,
	9516,
	9500,
	9472,
	9532,
	9566,
	9567,
	9562,
	9556,
	9577,
	9574,
	9568,
	9552,
	9580,
	9575,
	9576,
	9572,
	9573,
	9561,
	9560,
	9554,
	9555,
	9579,
	9578,
	9496,
	9484,
	9608,
	9604,
	9612,
	9616,
	9600,
	945,
	223,
	915,
	960,
	931,
	963,
	181,
	964,
	934,
	920,
	937,
	948,
	8734,
	966,
	949,
	8745,
	8801,
	177,
	8805,
	8804,
	8992,
	8993,
	247,
	8776,
	176,
	8729,
	183,
	8730,
	8319,
	178,
	9632,
	9633
	};

	/// @brief stores data from any Asciir loadable texture files (.cart, .xp, .txt)
	class FileTexture : public Texture2D
	{
	public:
		FileTexture() = default;
		/// @brief sets the passed directory as the current file texture directory.
		FileTexture(const Path& file_dir) : m_file_dir(file_dir) {}

		~FileTexture() override { if (loaded()) unload(); }
		
		/// @brief the currently loaded texture file.  
		/// returns an empty path, if no file is currently loaded.
		Path dir() const { return m_file_dir; };

		/// @brief sets the directory of the file texture.
		/// @attention this method does not load the file texture into memory. Inorder for the file to be readable, one should call load().
		void setDir(const Path& file_dir) { m_file_dir = file_dir; }

		/// @brief load the passed file path into memory
		/// 
		/// whenever a load funtion is called which takes more arguments than just the directory, the default arguments are used for the additional arguments. 
		/// 
		/// @return refrence to the current instance
		FileTexture& load(const Path& dir);

		/// @brief load the current file into memory
		/// @return refrence to the current instance
		FileTexture& load() { return load(m_file_dir); }

		/// @brief loads a cart texture file into memory.
		/// @return refrence to the current instance
		FileTexture& loadCART(const Path& cart_file);
		/// @brief loads a REXPaint texture file (.xp) into memory.
		/// @layer the layer that should be loaded into the file texture, by default this is the first layer
		/// @font_map a text file containing a mapping from the REXPaint symbol index to the actual UTF-8 decimal code point (NOT the byte sequence itself). 
		/// if nothing is passed, a default mapping will be used @see XP_FONT_MAP.
		/// @return refrence to the current instance
		FileTexture& loadXP(const Path& xp_file, size_t layer = 0, const std::array<uint32_t, 0x100>& font_map = XP_FONT_MAP);
		/// @brief loads a txt file into memory
		/// 
		/// no colour data is stored in txt file textures, instead txt files should be used when only symbols need to be loaded.
		/// the txt file should contain a rectangle of symbols, where each row is seperated by a newline.
		/// this method will automaticly determine the width and height of the texture, assuming the row and col sizes are consistent.
		/// 
		/// @return refrence to the current instance
		FileTexture& loadTXT(const Path& txt_file);
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
		FileTexture& loadIMG(const Path& img_file, bool use_half_tiles = false, bool load_foreground = false);

		/// @brief same as loadIMG() using the current file directory as the image directory.
		FileTexture& loadIMG(bool use_half_tiles = false, bool load_foreground = false) { return loadIMG(m_file_dir, use_half_tiles, load_foreground); }

		/// @brief unloads the texture and frees the previosly loaded memory
		FileTexture& unload();

		/// @brief reread the file texture into memory. Should be used if the file has been updated since the last load() / reload()
		FileTexture& reload();

		/// @brief returns wether the FileTexture currently has a texture loaded
		bool loaded() const { return m_is_loaded; }


	protected:
		Path m_file_dir = "";

		bool m_is_loaded = false;
	};

	/// @brief wrapper class of FileTexture specialized for reading sprite sheets.
	/// 
	/// use the constructor or the provided methods to configure the sprite sheet layout.
	/// one configured use setSprite() to set the sprite tile that readTile() should use.
	/// 
	/// when passing to Renderer::submit(), the getSprite() method should be used,
	/// as it only copies the selected sprite,instead of the entire sprite sheet, into the render buffer.
	/// 
	/// when a sprite sheet is tiled, only the active sprite is tiled.
	/// 
	class SpriteSheet : public FileTexture
	{
	public:
		/// @brief constructor
		/// @param file_dir the file that contains the sprite sheet
		/// @param sprite_size the size of a sprite tile in the sprite sheet
		/// @param grid_padding the padding inbetween sprite tiles
		/// @param grid_offset the offset, from the upper left corner, of which the sprite sheet starts
		SpriteSheet(const Path& file_dir, Size2D sprite_size, Size2D grid_padding = { 0, 0 }, Size2D grid_offset = { 0, 0 })
			: FileTexture(file_dir), m_sprite_size(sprite_size), m_padding(grid_padding), m_offset(grid_offset) {}

		/// @brief sets the sprite tile that will be considered the active sprite.
		void setSprite(Size2D sprite_pos);
		
		/// @brief converts the SpriteSheet instance to a Texture2D **only** conatining the active sprite
		Texture2D getSprite();
		/// @brief get the active tile postion
		TermVert getSpritePos() { return m_active_sprite; }

		/// @brief increase the active sprite tile by the passed amount.
		/// when increased, the tile will be moved to the right, and if out of bounds, will wrap around and move a row dowm.
		/// this is also true for when the final row is reached.
		void incrSprite(TInt amount = 1);
		/// @brief same as incrSprite, but the opposite direction
		/// equivalent to incrSprite(-amount)
		void decrSprite(TInt amount = 1) { incrSprite(-amount); }

		/// @brief set the size of a sprite tile
		void setSpriteSize(TermVert grid_size) { AR_ASSERT(grid_size.x > 0 && grid_size.y > 0); m_sprite_size = grid_size; }
		/// @brief get the size of a sprite tile
		TermVert getSpriteSize() const { return m_sprite_size; }

		/// @brief get the number of avaliable sprites in the x and y direction
		TermVert getSpriteCount() const { return (textureSize() - m_offset + m_padding).cwiseQuotient(m_sprite_size + m_padding); }

		void setOffset(TermVert offset) { AR_ASSERT(offset.x >= 0 && offset.y >= 0); m_offset = offset; }
		TermVert getOffset() { return m_offset; }

		void setPadding(TermVert padding) { AR_ASSERT(padding.x >= 0 && padding.y >= 0); m_padding = padding; }
		TermVert getPadding() const { return m_padding; }

		/// @brief the size of the active sprite, unless tiled size is not equal to (-1, -1).
		/// when the tiled size is anything else than (-1, -1), the size will be equal to the tiled size.
		TermVert size() const override;
		
		/// @brief read the tile at the passed coordinate, from the active tile.
		Tile readTile(const TermVert& coord, Coord uv = Coord(0, 0), const DeltaTime& time_since_start = 0, size_t frames_since_start = 0) override;

	protected:

		TermVert m_sprite_size;
		TermVert m_padding;
		TermVert m_offset;
		TermVert m_active_sprite;
	};

	template<typename TShader, typename Enable=void>
	class ShaderSequence;

	/// @brief allows tempalte to compile if TShader has Shader2D as a base class.
	template<typename TShader>
	using enable_if_shader = std::enable_if_t<std::is_base_of_v<Shader2D, TShader>>;

	/// @brief stores a sequence of Shaders.
	/// This class intended use is to store animations and to easily iterate through them.
	/// use nextFrame(), prevFrame() and setFrame(), to set what shader readTile will use.
	/// 
	/// the active frame is automaticly preserved whenever the frame container is modified (addFrame, removeFrame, etc...), so a call to any of these functions will not change the active frame.
	/// the only case where the active frame is changed, is if the current active frame is removed.
	/// in this case, the next valid frame to the left of the active frame, will be chosen as the next active frame.
	/// if there are no valid frames to the left of the active frame, then the next valid frame to the right of the active frame will be chosen.
	/// 
	template<typename TShader>
	class ShaderSequence<TShader, enable_if_shader<TShader>>
	{
	public:
		/// @brief default constructor, will contain 0 frames.
		ShaderSequence() = default;

		/// @brief initializes the TextureSequence with the passed values
		ShaderSequence(std::initializer_list<Ref<TShader>> init_values);

		ShaderSequence(const ShaderSequence<TShader>& other);

		/// @brief increases the current frame index by a specified amount.
		/// 
		/// if the final frame index is greater than the number of frames, then the frame index is wrapped around to the beginning of the TextureSequence.
		/// 
		/// @param jump_size the size of the increment
		/// @return refrence to the Texture2D at the destination position
		Ref<TShader> incrFrame(size_t jump_size = 1);

		/// @brief decreases the current frame index by a specified amount.
		/// 
		/// if the final frame index is greater than the number of frames, then the frame index is wrapped around to the end of the TextureSequence.
		/// 
		/// @param jump_size the size of the increment
		/// @return refrence to the Texture2D at the destination position
		Ref<TShader> decrFrame(size_t jump_size = 1);
		/// @brief sets the index of the active frame
		/// @return refrence to the Texture2D at the passed frame index
		Ref<TShader> setFrame(size_t frame) { AR_ASSERT(frame < m_frames.size()); m_curr_frame = frame; return m_frames[m_curr_frame]; }

		/// @brief gets the active frame
		Ref<TShader> getFrame() const { return m_frames[m_curr_frame]; }
		/// @brief gets the frame at the passed frame index
		Ref<TShader> getFrame(size_t frame) const { return m_frames[frame]; }

		/// @brief gets the number of frames in the current TextureSequence
		size_t frameCount() { return m_frames.size(); }

		/// @brief adds a Texture2D to the end of the texture sequence
		void addFrame(Ref<TShader> new_frame) { m_frames.push_back(new_frame); }
		/// @brief inserts a Texture2D at the specified position
		void addFrame(Ref<TShader> new_frame, size_t pos);

		/// @brief appends the passed frames to the end of the TextureSequence
		void addFrames(const std::vector<Ref<TShader>>& init_values);
		/// @brief inserts the passed frames at the specified position
		void addFrames(const std::vector<Ref<TShader>>& init_values, size_t pos);
		/// @brief same as @ref addFrames(other.data())
		void addFrames(const ShaderSequence<TShader>& other) { addFrames(other.data()); }
		/// @brief same as @ref addFrames(other.data(), pos)
		void addFrames(const ShaderSequence<TShader>& other, size_t pos) { addFrames(other.data(), pos); }

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
		const std::vector<Ref<TShader>>& data() const { return m_frames; }

		/// @brief same as getFrame()
		operator Ref<TShader>() { return getFrame(); }

	protected:
		size_t m_curr_frame = 0;
		std::vector<Ref<TShader>> m_frames;
	};
	
	typedef ShaderSequence<FileTexture> ImageSequence;
}

#include "Texture.ipp"
