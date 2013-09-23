#include "gpu.h"

#include "SDL.h"

GPU::GPU() { }

GPU::~GPU() 
{
	SDL_Quit();
}

bool GPU::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
}
