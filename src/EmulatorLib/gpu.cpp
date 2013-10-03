#include "gpu.h"

#include "SDL.h"

GPU::GPU() { }

GPU::~GPU() 
{
	SDL_Quit();
}

bool GPU::Init()
{
	// Q : Correct condition ??
	return true;
	//return SDL_Init(SDL_INIT_VIDEO) > 0;
}

void GPU::ClearScreen() { /*TODO*/ }

void GPU::Draw(Int16 in_X, Int16 in_Y, UInt16 in_Sprite) { /*TODO*/ }

void GPU::Flip(UInt8 in_H, UInt8 in_V) { /*TODO*/ }

void GPU::LoadPalette(const UInt8 in_Palette[16][3])  { /*TODO*/ }

void GPU::SetBackgroundColor(UInt8 in_ColorIndex) { /*TODO*/ }

void GPU::SetSpriteDimensions(UInt8 in_Width, UInt8 in_Height) { /*TODO*/ }
