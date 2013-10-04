#include <iostream>

#include "commandlineinterface.h"
#include "emulator.h"

/**
* \fn main
* \brief Emulator application driver
* \param argc Number of arguments given on the command line
* \param argv Arguments given on the command line
*/
int main(int argc, char** argv)
{
	CommandLineInterface l_CmdI;

	std::string l_ROM = l_CmdI.ChooseROM();

	if(!l_ROM.empty())
	{
		Emulator l_Emu;
		// TODO : Error reporting
		l_Emu.Init(l_ROM);
		l_Emu.Emulate();
	}
	else
	{
		std::cout << "Unknown option" << std::endl;
	}

	std::cout << "Exiting" << std::endl;
	return 0;
}
