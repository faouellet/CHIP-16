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
		unsigned l_ErrorCode = l_Emu.Init(l_ROM);
		if(!l_ErrorCode)
		{
			l_Emu.Emulate(false);
		}
		else
		{
			if(l_ErrorCode & EmptyROMError)
				std::cout << "ROM doesn't contain any data" << std::endl;
			if(l_ErrorCode & ROMOverflowError)
				std::cout << "ROM too big, doesn't fit into memory" << std::endl;
			if(l_ErrorCode & GPUInitError)
				std::cout << "Problem initializing the GPU" << std::endl;
			if(l_ErrorCode & GPUWindowCreationError)
				std::cout << "Problem while creating the emulator window" << std::endl;
			if(l_ErrorCode & SPUError)
				std::cout << "SPU Error" << std::endl;
			if(l_ErrorCode & FileError)
				std::cout << "File not found" << std::endl;
		}
	}
	else
	{
		std::cout << "Unknown option" << std::endl;
	}

	std::cout << "Exiting" << std::endl;
	return 0;
}
