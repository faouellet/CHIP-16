#include "gpu.h"

#include <functional>
#include <vector>

// TODO : Document the usage of the SDL functions


GPU::GPU() : m_Window(std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow)),
	m_Renderer(std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer)),
	m_Texture(std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture))
{
	SetOriginalColorPalette();
}

GPU::~GPU() 
{
	SDL_Quit();
}

void GPU::SetOriginalColorPalette()
{
	m_Colors[0] = 0x000000;		// Black (Transparent in foreground layer)
	m_Colors[1] = 0x000000;		// Black
	m_Colors[2] = 0x888888;		// Gray
	m_Colors[3] = 0xBF3932;		// Red
	m_Colors[4] = 0xDE7AAE;		// Pink
	m_Colors[5] = 0x4C3D21;		// Dark Brown
	m_Colors[6] = 0x905F25;		// Brown
	m_Colors[7] = 0xE49452;		// Orange
	m_Colors[8] = 0xEAD979;		// Yellow
	m_Colors[9] = 0x537A3B;		// Green
	m_Colors[10] = 0xABD54A;	// Light Green
	m_Colors[11] = 0x252E38;	// Dark Blue
	m_Colors[12] = 0x00467F;	// Blue
	m_Colors[13] = 0x68ABCC;	// Light Blue
	m_Colors[14] = 0xBCDEE4;	// Sky Blue
	m_Colors[15] = 0xFFFFFF;	// White
}

unsigned GPU::Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return Utils::GPUInitError;

	m_Window.reset(SDL_CreateWindow("Chip16", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_MAXIMIZED));
	if(m_Window == nullptr)
		return Utils::GPUWindowCreationError;

	m_Renderer.reset(SDL_CreateRenderer(m_Window.get(), -1, 0));
	if(m_Renderer == nullptr)
		return Utils::GPURendererError;

	m_Texture.reset(SDL_CreateTexture(m_Renderer.get(), SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT));
	if(m_Texture == nullptr)
		return Utils::GPUTextureError;

	return Utils::NoError;
}

void GPU::Reset()
{
	SetOriginalColorPalette();
}

UInt16 GPU::SpriteHeight() const
{
	return m_Sprite.Height;
}

UInt16 GPU::SpriteWidth() const
{
	return m_Sprite.Width;
}

void GPU::TurnOffVBlankFlag()
{
	m_VBlankFlag = 0;
}

Utils::UInt8 GPU::VBlankFlag() const
{
	return m_VBlankFlag;
}

void GPU::ClearScreen() 
{
	SDL_RenderClear(m_Renderer.get());
}

UInt8 GPU::Draw(Int16 in_X, Int16 in_Y, const std::vector<UInt8> & in_Sprite) 
{
	// Validate X and Y coordinates
	if(in_X > WIDTH - 1 || in_Y > HEIGHT -1)
		return 0;

	// Deal with the horizontal and vertical flip
	std::function<UInt16(int)> l_XIndex;
	std::function<UInt16(int)> l_YIndex;
	UInt16 l_XStart, l_YStart;

	if(m_Sprite.FlipHorizontal)
	{
		l_XStart = in_X + m_Sprite.Width;
		l_XIndex = [l_XStart](int i){ return l_XStart - i; };
	}
	else
	{
		l_XStart = in_X;
		l_XIndex = [l_XStart](int i){ return l_XStart + i; };
	}

	if(m_Sprite.FlipVertical)
	{
		l_YStart = in_Y + m_Sprite.Height;
		l_YIndex = [l_YStart](int i){ return l_YStart - i; };
	}
	else
	{
		l_YStart = in_Y;
		l_YIndex = [l_YStart](int i){ return l_YStart + i; };
	}

	UInt8 l_Hit = 0;
	for(int i = 0; i < m_Sprite.Width; ++i)
	{
		for(int j = 0; j < m_Sprite.Height; ++j)
		{
			// TODO : Test hits with other sprites
			l_Hit += m_ScreenBuffer[l_XIndex(i)][l_YIndex(j)];

			m_ScreenBuffer[l_XIndex(i)][l_YIndex(j)] = in_Sprite[i * m_Sprite.Height + j];
		}
	}

	return l_Hit;
}

void GPU::Flip(UInt8 in_H, UInt8 in_V) 
{
	m_Sprite.FlipHorizontal = (0 != (in_H & 0xF));
	m_Sprite.FlipVertical = (0 != (in_V & 0xF));
}

void GPU::FlushBuffer()
{
	// Q : What to do with the current screen buffer after ???
	SDL_UpdateTexture(m_Texture.get(), nullptr, m_ScreenBuffer, WIDTH * sizeof(UInt32));
	SDL_RenderClear(m_Renderer.get());
	SDL_RenderCopy(m_Renderer.get(), m_Texture.get(), nullptr, nullptr);
	SDL_RenderPresent(m_Renderer.get());

	m_VBlankFlag = 1;
}

void GPU::LoadPalette(const UInt8 in_Palette[16][3])  
{
	for(int i = 0; i < 16; ++i)
		m_Colors[i] = in_Palette[i][0] << 16 | in_Palette[i][1] << 8 | in_Palette[i][2]; 
}

void GPU::SetBackgroundColor(UInt8 in_ColorIndex) 
{
	SDL_SetRenderDrawColor(m_Renderer.get(), m_Colors[in_ColorIndex] & 0xFF0000,
		m_Colors[in_ColorIndex] & 0x00FF00, m_Colors[in_ColorIndex] & 0x0000FF, 255);
}

void GPU::SetSpriteDimensions(UInt8 in_Width, UInt8 in_Height) 
{
	m_Sprite.Width = in_Width * 2;
	m_Sprite.Height = in_Height;
}
