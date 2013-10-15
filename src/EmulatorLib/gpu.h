#ifndef GPU_H
#define GPU_H

#include <memory>
#include <vector>

#include "SDL.h"

#include "utils.h"

using Utils::UInt8;
using Utils::Int16;
using Utils::UInt16;
using Utils::UInt32;

/**
* \class GPU
* \brief Emulates the Graphics Processing Unit (GPU)
* \author Felix-Antoine Ouellet
*/
class GPU
{
private:
	/**
	* \enum
	* \brief Useful constants
	*/
	enum { NB_COLORS = 16, HEIGHT = 240, WIDTH = 320 };

private:
	/**
	* \struct
	* \brief Aggregate of the properties of a pre-rendered figure. This will 
	*        serve for all sprites to draw at a given frame
	*/
	struct Sprite
	{
		bool FlipHorizontal;	/*!< True = Sprite oriented right \n False = Sprite oriented left */
		bool FlipVertical;		/*!< True = Sprite oriented upward \n False = Sprite oriented downward */
		UInt8 Height;			/*!< Height of the sprite in number of pixels */
		UInt8 Width;			/*!< Width of the sprite in number of pixels */

		/**
		* \fn Sprite
		* \brief Default constructor
		*/
		Sprite() : FlipHorizontal(false), FlipVertical(false), Height(0), Width(0) { }
		
		/**
		* \fn ~Sprite
		* \brief Destructor
		*/
		~Sprite() { }
	};

private:
	// Q : UInt32 or UInt8 for the screen buffer ???
	UInt32 m_ScreenBuffer[HEIGHT][WIDTH];		/*!< Buffer to be drawn */
	UInt32 m_ScreenColors[HEIGHT][WIDTH];		/*!< Actual graphics to be drawn */

	UInt8 m_SpritesVerticalOrientation;			/*!< Vertical orientation of all the sprites */
	UInt8 m_SpritesHorizontalOrientation;		/*!< Horizontal orientation of all the sprites */

	UInt8 m_VBlankFlag;							/*!< VBlank Flag */
	
	UInt32 m_Colors[NB_COLORS];					/*!< Palette of colors used when drawing a frame */

	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> m_Window;			/*!< Emulator window */
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> m_Renderer;	/*!< Renderer abstracting away the details of how a window is drawn */
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> m_Texture;		/*!< Abstaction over the raw s */

	Sprite m_Sprite;							/*!< Abstraction of the properties of all the sprites */

public:
	/**
	* \fn GPU
	* \brief Default constructor
	*/
	GPU();

	/**
	* \fn ~GPU
	* \brief Destructor
	*/
	~GPU();
	
private:
	/**
	* \fn SetOriginalColorPalette
	* \brief Set the color palette to its original state
	*/
	void SetOriginalColorPalette();

public:
	/**
	* \fn Init
	* \brief Initialize the graphics processing unit
	* \return Error code
	*/
	unsigned Init();

	/**
	* \fn Reset
	* \brief Restore the graphics processing unit at its pre-initialized state
	*/
	void Reset();

	/**
	* \fn SpriteHeight
	* \brief Get the height for all the sprite
	* \return The value of the height
	*/
	UInt16 SpriteHeight() const;

	/**
	* \fn SpriteWidth
	* \brief Get the width for all the sprite
	* \return The value of the width
	*/
	UInt16 SpriteWidth() const;

	/**
	* \fn TurnOffVBlankFlag
	* \brief Set the VBlank flag to 0
	*/
	void TurnOffVBlankFlag();

	/**
	* \fn VBlankFlag
	* \brief Get the value of the vblank flag
	* \return The value of the vblank flag
	*/
	Utils::UInt8 VBlankFlag() const;

public:
	/**
	* \fn ClearScreen
	* \brief Erase foreground layer and set background to black
	*/
	void ClearScreen();

	/**
	* \fn Draw
	* \brief Prepare the screen buffer to draw a sprite
	* \param in_X The X coordinate of where to draw the sprite
	* \param in_Y The Y coordinate of where to draw the sprite
	* \param in_Sprite The sprite to be drawn on the screen
	* \return Any sprite hit when drawing
	*/
	bool Draw(Int16 in_X, Int16 in_Y, const std::vector<UInt8> & in_Sprite);

	/**
	* \fn Flip
	* \brief Set the flip orientation for sprites
	* \param in_H Horizontal orientation
	* \param in_V Verticl orientation
	*/
	void Flip(UInt8 in_H, UInt8 in_V);

	/**
	* \fn FlushBuffer
	* \brief Draw a frame by flushing the current screen buffer
	*/
	void FlushBuffer();

	/**
	* \fn LoadPalette
	* \brief Load a palette
	* \param in_Palette 16*3 bytes representing the palette data
	*/
	void LoadPalette(const UInt8 in_Palette[16][3]);

	/**
	* \fn SetBackgroundColor
	* \brief Set background color to index N
	* \param in_ColorIndex The color index
	*/
	void SetBackgroundColor(UInt8 in_ColorIndex);

	/**
	* \fn SetSpriteDimensions
	* \brief Set the sprite witdt and height to new dimensions
	* \param in_Height The new sprite height
	* \param in_Width The new sprite width
	*/
	void SetSpriteDimensions(UInt8 in_Height, UInt8 in_Width);
};

#endif // GPU_H
