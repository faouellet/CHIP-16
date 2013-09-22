#include <iostream>
#include <map>
#include <string>

#include "SDL.h"

// TODO : I think this should go into the GPU emulation
void TestSDL()
{
	SDL_Window* l_Window = nullptr;

	l_Window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Renderer* l_Renderer = nullptr;

	l_Renderer = SDL_CreateRenderer(l_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Texture* l_Tex = SDL_CreateTextureFromSurface(l_Renderer, nullptr);
	
	SDL_RenderClear(l_Renderer);
	SDL_RenderCopy(l_Renderer, l_Tex, nullptr, nullptr);
	SDL_RenderPresent(l_Renderer);

	SDL_Delay(2000);

	SDL_DestroyTexture(l_Tex);
	SDL_DestroyRenderer(l_Renderer);
	SDL_DestroyWindow(l_Window);
	SDL_Quit();
}

// TODO1 : At least validate the input
// TODO2 : Change it to a GUI
std::string ChooseROM()
{
	static std::map<int, std::string> l_DemoChoices = { {1,"AdsrDemo"}, {2, "Anim"}, {3,"ASCII"}, {4,"Ball"}, 
														{5,"GB16"}, {6,"Intro"}, {7,"Mandel"}, {8,"Maze"}, 
														{9,"SongOfStorms"}, {10,"Starfield" }, {11,"Static"}, 
														{12,"Stopwatch"}, {13,"Triangle"} };
	static std::map<int, std::string> l_GameChoices = { {1,"Herdle"}, {2,"MusicMaker"}, {3,"Ninja"}, {4,"Pong"}, 
														{5,"Reflection"}, {6,"Snafu"} };
	static std::map<int, std::string> l_TestChoices = { {1,"AdsrTest"}, {2,"BC_TestRom"}, {3,"CollisionTest"}, 
														{4,"flip_test"}, {5,"PadTest"}, {6,"PaleteTest"}, 
														{7,"PaletteFlip"}, {8,"SoundTest"} };

	std::cout << "Choose between 1- Demos 2- Games 3- Tests" << std::endl;
	unsigned l_Choice = 0;
	std::cin >> l_Choice;

	std::string l_ROM = "";

	switch (l_Choice)
	{
		case 1:
		{
			std::cout << "Choose a demo to run" << std::endl;
			for(auto& l_DemoChoice : l_DemoChoices)
				std::cout << l_DemoChoice.first << "- " << l_DemoChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Demos/" + l_GameChoices[l_Choice];
		}
		case 2:
		{
			std::cout << "Choose a game to run" << std::endl;
			for(auto& l_GameChoice : l_GameChoices)
				std::cout << l_GameChoice.first << "- " << l_GameChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Games/" + l_GameChoices[l_Choice];
		}
		case 3:
		{
			std::cout << "Choose a test to run" << std::endl;
			for(auto& l_TestChoice : l_TestChoices)
				std::cout << l_TestChoice.first << "- " << l_TestChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Tests/" + l_GameChoices[l_Choice];
		}
		default:
		{
			std::cout << "Unknown option" << std::endl;
		}
	}

	return l_ROM;
}

int main(int argc, char** argv)
{
	std::string l_ROM = ChooseROM();

	// TODO : Load it to memory and start emulating

	return 0;
}
