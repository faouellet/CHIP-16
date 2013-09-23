#include <iostream>
#include <map>

#include "emulator.h"


// TODO 1 : At least validate the input
// TODO 2 : Change it to a GUI
std::string ChooseROM()
{
	// TODO : Make this portable for at least vc11
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

/**
* \fn main
* \brief Main application driver
*/
int main(int argc, char** argv)
{
	std::string l_ROM = ChooseROM();

	Emulator l_Emu;
	// TODO : Error reporting
	l_Emu.Init(l_ROM);
	l_Emu.Emulate();

	return 0;
}
