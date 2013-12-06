#include "commandlineinterface.h"

#include <iostream>
#include <sstream>

CommandLineInterface::CommandLineInterface()
{
	m_DemoChoices.push_back("AdsrDemo");
	m_DemoChoices.push_back("Anim");
	m_DemoChoices.push_back("ASCII");
	m_DemoChoices.push_back("Ball");
	m_DemoChoices.push_back("boing");
	m_DemoChoices.push_back("dots2");
	m_DemoChoices.push_back("GB16");
	m_DemoChoices.push_back("Intro");
	m_DemoChoices.push_back("Mandel");
	m_DemoChoices.push_back("Maze");
	m_DemoChoices.push_back("Starfield");
	m_DemoChoices.push_back("Static");
	m_DemoChoices.push_back("Stopwatch");
	m_DemoChoices.push_back("Triangle");
	m_DemoChoices.push_back("Water");
	m_DemoChoices.push_back("XOR");
	m_DemoChoices.push_back("Tetris");

	m_GameChoices.push_back("Alien");
	m_GameChoices.push_back("Herdle");
	m_GameChoices.push_back("MusicMaker");
	m_GameChoices.push_back("Ninja");
	m_DemoChoices.push_back("Pacman");
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

	m_Choices.push_back(std::make_pair("Demo", m_DemoChoices));
	m_Choices.push_back(std::make_pair("Game", m_GameChoices));
	m_Choices.push_back(std::make_pair("Test", m_TestChoices));
}

CommandLineInterface::~CommandLineInterface() { }

std::string CommandLineInterface::ChooseROM()
{
	std::cout << "Choose between 1- Demos 2- Games 3- Tests" << std::endl;
	unsigned l_Choice = 0;
	std::cin >> l_Choice;

	std::string l_ROM;

	if(0 < l_Choice && l_Choice < 4)
	{
		std::cout << "Choose a " << m_Choices[--l_Choice].first << " to run" << std::endl;
		unsigned l_GameChoice = Choose(m_Choices[l_Choice].second);
		if(0 <= l_GameChoice && l_GameChoice < m_Choices[l_Choice].second.size())
			l_ROM = FormatChoice(m_Choices[l_Choice].first, m_Choices[l_Choice].second[l_GameChoice]);
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
	l_SStream << "./" << in_Folder << "s/" << in_Choice << "/" << in_Choice << ".c16";
	return l_SStream.str();
}
