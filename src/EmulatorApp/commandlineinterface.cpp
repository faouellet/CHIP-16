#include "commandlineinterface.h"

#include <iostream>
#include <sstream>

CommandLineInterface::CommandLineInterface()
{
	m_DemoChoices.push_back("AdsrDemo");
	m_DemoChoices.push_back("Anim");
	m_DemoChoices.push_back("ASCII");
	m_DemoChoices.push_back("Ball");
	m_DemoChoices.push_back("GB16");
	m_DemoChoices.push_back("Intro");
	m_DemoChoices.push_back("Mandel");
	m_DemoChoices.push_back("Maze");
	m_DemoChoices.push_back("SongOfStorms");
	m_DemoChoices.push_back("Starfield");
	m_DemoChoices.push_back("Static");
	m_DemoChoices.push_back("Stopwatch");
	m_DemoChoices.push_back("Triangle");

	m_GameChoices.push_back("Herdle");
	m_GameChoices.push_back("MusicMaker");
	m_GameChoices.push_back("Ninja");
	m_GameChoices.push_back("Pong");
	m_GameChoices.push_back("Reflection");
	m_GameChoices.push_back("Snafu");

	m_TestChoices.push_back("AdsrTest");
	m_TestChoices.push_back("BC_TestRom");
	m_TestChoices.push_back("CollisionTest");
	m_TestChoices.push_back("flip_test");
	m_TestChoices.push_back("PadTest");
	m_TestChoices.push_back("PaleteTest");
	m_TestChoices.push_back("PaletteFlip");
	m_TestChoices.push_back("SoundTest");
}

CommandLineInterface::~CommandLineInterface() { }

std::string CommandLineInterface::ChooseROM()
{
	// TODO : Could probably still be refactored
	std::cout << "Choose between 1- Demos 2- Games 3- Tests" << std::endl;
	unsigned l_Choice = 0;
	std::cin >> l_Choice;

	std::string l_ROM;

	switch (l_Choice)
	{
		case 1:
		{
			std::cout << "Choose a demo to run" << std::endl;
			l_Choice = Choose(m_DemoChoices);
			if(0 <= l_Choice && l_Choice < m_DemoChoices.size())
				l_ROM = FormatChoice("Demos", m_DemoChoices[l_Choice]);
			break;
		}
		case 2:
		{
			std::cout << "Choose a game to run" << std::endl;
			l_Choice = Choose(m_GameChoices);
			if(0 <= l_Choice && l_Choice < m_GameChoices.size())
				l_ROM = FormatChoice("Games", m_GameChoices[l_Choice]);
			break;
		}
		case 3:
		{
			std::cout << "Choose a test to run" << std::endl;
			l_Choice = Choose(m_TestChoices);
			if(0 <= l_Choice && l_Choice < m_TestChoices.size())
				l_ROM = FormatChoice("Tests", m_TestChoices[l_Choice]);
			break;
		}
	}
	return l_ROM;
}

int CommandLineInterface::Choose(const std::vector<std::string> in_Choices) const
{
	for(unsigned i = 0; i < in_Choices.size(); ++i)
		std::cout << i << "- " << in_Choices[i] << std::endl;

	int l_Choice;
	std::cin >> l_Choice;
	return l_Choice;
}

std::string CommandLineInterface::FormatChoice(const std::string & in_Folder, 
											   const std::string & in_Choice) const
{
	std::stringstream l_SStream;
	l_SStream << in_Folder << "/" << in_Choice << "/" << in_Choice << ".c16";
	return l_SStream.str();
}
