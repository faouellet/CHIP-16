#ifndef GPU_H
#define GPU_H

#include <vector>

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
	// TODO : ...
	// Most certainly a screen buffer
	std::vector<UInt32> m_ColorIndexes;

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
	
public:
	/**
	* \fn Init
	* \brief Initialize the graphics processing unit
	* \return Success or failure
	*/
	bool Init();

public:
	/**
	* \fn ClearScreen
	* \brief Erase foreground layer and set background to black
	*/
	void ClearScreen();

	/**
	* \fn Draw
	* \brief Draw a sprite on the screen
	* \param in_X The X coordinate of where to draw the sprite
	* \param in_Y The Y coordinate of where to draw the sprite
	* \param in_Sprite The sprite to draw on the screen
	*/
	void Draw(Int16 in_X, Int16 in_Y, UInt16 in_Sprite);

	/**
	* \fn Flip
	* \brief Set the flip orientation for sprites
	* \param in_H Horizontal orientation
	* \param in_V Verticl orientation
	*/
	void Flip(UInt8 in_H, UInt8 in_V);

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
	* \param in_Width The new sprite width
	* \param in_Height The new sprite height
	*/
	void SetSpriteDimensions(UInt8 in_Width, UInt8 in_Height);
};

#endif // GPU_H
