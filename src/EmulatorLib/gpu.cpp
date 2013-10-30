#include "gpu.h"

#include <functional>
#include <vector>

#include <iostream>

// TODO : Document the usage of the SDL functions

GPU::GPU() : m_Window(std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow)),
	m_Renderer(std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer)),
	m_Texture(std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>(nullptr, SDL_DestroyTexture)),
	m_BGC(0)
{
	SetOriginalColorPalette();
	// Paint it black
	memset(m_ScreenBuffer, 0, sizeof(UInt32)*HEIGHT*WIDTH);
	memset(m_ScreenColors, 0, sizeof(UInt32)*HEIGHT*WIDTH);
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
	m_Colors[3] = 0x3239BF;		// Red
	m_Colors[4] = 0xAE7ADE;		// Pink
	m_Colors[5] = 0x213D4C;		// Dark Brown
	m_Colors[6] = 0x255F90;		// Brown
	m_Colors[7] = 0x5294E4;		// Orange
	m_Colors[8] = 0x79D9EA;		// Yellow
	m_Colors[9] = 0x3B7A53;		// Green
	m_Colors[10] = 0x4AD5AB;	// Light Green
	m_Colors[11] = 0x382E25;	// Dark Blue
	m_Colors[12] = 0x7F4600;	// Blue
	m_Colors[13] = 0xCCAB68;	// Light Blue
	m_Colors[14] = 0xE4DEBC;	// Sky Blue
	m_Colors[15] = 0xFFFFFF;	// White
}

std::vector<UInt32> GPU::DumpColors() const
{
	return std::vector<UInt32>(std::begin(m_Colors), std::end(m_Colors));
}

std::vector<UInt32> GPU::DumpScreenBuffer() const
{
	std::vector<UInt32> l_Return(HEIGHT * WIDTH);

	for(int i = 0; i < HEIGHT; ++i)
		for(int j = 0; j < WIDTH; ++j)
			l_Return[i * HEIGHT + j] = m_ScreenBuffer[i][j];

	return l_Return;
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
	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			m_ScreenColors[i][j] = m_Colors[m_BGC];
			m_ScreenBuffer[i][j] = 0;
		}
	}
	m_BGC = 0;
}

bool GPU::Draw(Int16 in_X, Int16 in_Y, const std::vector<UInt8> & in_Sprite) 
{
	// FIXME : Sprite appears at the opposite Y coordinate / or are they bouncing? 
	
	// Validate X and Y coordinates
	if(in_X > WIDTH - 1 || in_Y > HEIGHT -1 || in_X < -1 || in_Y < -1)
		return 0;

	// Allow to draw to negative coordinates
	UInt16 l_X = std::abs(in_X);
	UInt16 l_Y = std::abs(in_Y);

	// Deal with the horizontal and vertical flip
	Int16 l_XStart, l_YStart;
	Int16 l_XEnd, l_YEnd;
	Int16 l_XInc, l_YInc;

	if(m_Sprite.FlipHorizontal)
	{
		l_XStart = m_Sprite.Width - 1;
		l_XEnd = -1;
		l_XInc = -1;
	}
	else
	{
		l_XStart = 0;
		l_XEnd = m_Sprite.Width;
		l_XInc = 1;
	}
	if(m_Sprite.FlipVertical)
	{
		l_YStart = m_Sprite.Height - 1;
		l_YEnd = -1;
		l_YInc = -1;
	}
	else
	{
		l_YStart = 0;
		l_YEnd = m_Sprite.Height;
		l_YInc = 1;
	}

	UInt8 l_Hit = 0;
	for(int i = l_YStart, k = 0; i < l_YEnd, k < m_Sprite.Height; i+=l_YInc, ++k)
	{
		for(int j = l_XStart, l = 0; j < l_XEnd, l < m_Sprite.Width; j+=l_XInc, ++l)
		{
			// Test hits with other sprites
			l_Hit += m_ScreenBuffer[i+l_Y][j+l_X] == 0 ? 0 : 1;
			if(in_Sprite[i * m_Sprite.Width + j])
				m_ScreenBuffer[i+l_Y][j+l_X] = in_Sprite[k * m_Sprite.Width + l];
		}
	}

	return l_Hit > 0;
}

void GPU::Flip(UInt8 in_NewOrientation) 
{
	m_Sprite.FlipHorizontal = in_NewOrientation & 0x2;
	m_Sprite.FlipVertical = in_NewOrientation & 0x1;
}

void GPU::FlushBuffer()
{
	for(int i = 0; i < HEIGHT; ++i)
		for(int j = 0; j < WIDTH; ++j)
			m_ScreenColors[i][j] = m_ScreenBuffer[i][j] ? m_Colors[m_ScreenBuffer[i][j]] : m_Colors[m_BGC];
	
	SDL_UpdateTexture(m_Texture.get(), nullptr, m_ScreenColors, WIDTH * sizeof(UInt32));
	SDL_RenderClear(m_Renderer.get());
	SDL_RenderCopy(m_Renderer.get(), m_Texture.get(), nullptr, nullptr);
	SDL_RenderPresent(m_Renderer.get());

	m_VBlankFlag = 1;
}

void GPU::LoadPalette(const std::vector<UInt8> in_Palette)  
{
	for(int i = 0; i < 16*3; i+=3)
		m_Colors[i] = in_Palette[i] << 16 | in_Palette[i+1] << 8 | in_Palette[i+2]; 
}

void GPU::SetBackgroundColor(UInt8 in_ColorIndex) 
{
	m_BGC = in_ColorIndex;
}

void GPU::SetSpriteDimensions(UInt8 in_Height, UInt8 in_Width) 
{
	m_Sprite.Width = in_Width * 2;
	m_Sprite.Height = in_Height;
}
