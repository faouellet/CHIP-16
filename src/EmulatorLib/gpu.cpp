#include "gpu.h"

#include "SDL.h"

const UInt8 GPU::M_HEIGHT = 240;
const UInt8 GPU::M_WIDTH = 320;

GPU::GPU() 
{
	m_ColorIndexes.push_back(0x000000);		// Black (Transparent in foreground layer)
	m_ColorIndexes.push_back(0x000000);		// Black
	m_ColorIndexes.push_back(0x888888);		// Gray
	m_ColorIndexes.push_back(0xBF3932);		// Red
	m_ColorIndexes.push_back(0xDE7AAE);		// Pink
	m_ColorIndexes.push_back(0x4C3D21);		// Dark Brown
	m_ColorIndexes.push_back(0x905F25);		// Brown
	m_ColorIndexes.push_back(0xE49452);		// Orange
	m_ColorIndexes.push_back(0xEAD979);		// Yellow
	m_ColorIndexes.push_back(0x537A3B);		// Green
	m_ColorIndexes.push_back(0xABD54A);		// Light Green
	m_ColorIndexes.push_back(0x252E38);		// Dark Blue
	m_ColorIndexes.push_back(0x00467F);		// Blue
	m_ColorIndexes.push_back(0x68ABCC);		// Light Blue
	m_ColorIndexes.push_back(0xBCDEE4);		// Sky Blue
	m_ColorIndexes.push_back(0xFFFFFF);		// White
}

GPU::~GPU() 
{
	SDL_Quit();
}

Utils::UInt8 GPU::VBlankFlag()
{
	return m_VBlankFlag;
}

unsigned GPU::Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return Utils::GPUInitError;
	if((m_Window = SDL_CreateWindow("Chip16", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, M_WIDTH, M_HEIGHT, SDL_WINDOW_MAXIMIZED))
		== nullptr)
		return Utils::GPUWindowCreationError;
	return Utils::NoError;
}

void GPU::ClearScreen() { /*TODO*/ }

void GPU::Draw(Int16 in_X, Int16 in_Y, UInt16 in_Sprite) { /*TODO*/ }

void GPU::Flip(UInt8 in_H, UInt8 in_V) { /*TODO*/ }

void GPU::LoadPalette(const UInt8 in_Palette[16][3])  { /*TODO*/ }

void GPU::SetBackgroundColor(UInt8 in_ColorIndex) { /*TODO*/ }

void GPU::SetSpriteDimensions(UInt8 in_Width, UInt8 in_Height) { /*TODO*/ }
