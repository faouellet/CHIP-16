#include "SDL.h"

int main(int argc, char** argv)
{
	SDL_Window* l_Window = nullptr;

	l_Window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Renderer* l_Renderer = nullptr;

	l_Renderer = SDL_CreateRenderer(l_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Surface* l_Img = SDL_LoadBMP("errors.png");

	SDL_Texture* l_Tex = SDL_CreateTextureFromSurface(l_Renderer, l_Img);

	SDL_FreeSurface(l_Img);

	SDL_RenderClear(l_Renderer);
	SDL_RenderCopy(l_Renderer, l_Tex, nullptr, nullptr);
	SDL_RenderPresent(l_Renderer);

	SDL_Delay(2000);

	SDL_DestroyTexture(l_Tex);
	SDL_DestroyRenderer(l_Renderer);
	SDL_DestroyWindow(l_Window);
	SDL_Quit();

	return 0;
}
