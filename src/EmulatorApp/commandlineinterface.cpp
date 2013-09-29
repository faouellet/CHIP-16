#include "commandlineinterface.h"

#include <iostream>

CommandLineInterface::CommandLineInterface()
{
	m_DemoChoices[1] = "AdsrDemo";
	m_DemoChoices[2] = "Anim";
	m_DemoChoices[3] = "ASCII";
	m_DemoChoices[4] = "Ball";
	m_DemoChoices[5] = "GB16";
	m_DemoChoices[6] = "Intro";
	m_DemoChoices[7] = "Mandel";
	m_DemoChoices[8] = "Maze";
	m_DemoChoices[9] = "SongOfStorms";
	m_DemoChoices[10] = "Starfield";
	m_DemoChoices[11] = "Static";
	m_DemoChoices[12] = "Stopwatch";
	m_DemoChoices[13] = "Triangle";

	m_GameChoices[1] = "Herdle";
	m_GameChoices[2] = "MusicMaker";
	m_GameChoices[3] = "Ninja";
	m_GameChoices[4] = "Pong";
	m_GameChoices[5] = "Reflection";
	m_GameChoices[6] = "Snafu";

	m_TestChoices[1] = "AdsrTest";
	m_TestChoices[2] = "BC_TestRom";
	m_TestChoices[3] = "CollisionTest";
	m_TestChoices[4] = "flip_test";
	m_TestChoices[5] = "PadTest";
	m_TestChoices[6] = "PaleteTest";
	m_TestChoices[7] = "PaletteFlip";
	m_TestChoices[8] = "SoundTest";
}

CommandLineInterface::~CommandLineInterface() { }

std::string CommandLineInterface::ChooseROM()
{
	std::cout << "Choose between 1- Demos 2- Games 3- Tests" << std::endl;
	unsigned l_Choice = 0;
	std::cin >> l_Choice;

	std::string l_ROM = "";

	switch (l_Choice)
	{
		case 1:
		{
			std::cout << "Choose a demo to run" << std::endl;
			for(auto& l_DemoChoice : m_DemoChoices)
				std::cout << l_DemoChoice.first << "- " << l_DemoChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Demos/" + m_DemoChoices[l_Choice];
			break;
		}
		case 2:
		{
			std::cout << "Choose a game to run" << std::endl;
			for(auto& l_GameChoice : m_GameChoices)
				std::cout << l_GameChoice.first << "- " << l_GameChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Games/" + m_GameChoices[l_Choice];
			break;
		}
		case 3:
		{
			std::cout << "Choose a test to run" << std::endl;
			for(auto& l_TestChoice : m_TestChoices)
				std::cout << l_TestChoice.first << "- " << l_TestChoice.second << std::endl;

			std::cin >> l_Choice;
			l_ROM = "Tests/" + m_TestChoices[l_Choice];
			break;
		}
		default:
		{
			std::cout << "Unknown option" << std::endl;
		}
	}
	return l_ROM;
}