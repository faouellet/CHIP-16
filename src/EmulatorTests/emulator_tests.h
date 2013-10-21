#ifndef EMULATOR_TESTS_H
#define EMULATOR_TESTS_H

#include <string>

#include "emulator.h"

/**
* \struct EmulatorTestFixture
* \brief Fixture containing the information needed to run the test ROMs included
*        in the program pack
* \author Felix-Antoine Ouellet
*/
struct EmulatorTestFixture
{
	static const std::string ROM_PATH;
	
	Emulator Emu;

	EmulatorTestFixture() { }
};

#ifdef _DEBUG
//const std::string EmulatorTestFixture::ROM_PATH = "../bin/Debug/Tests";
const std::string EmulatorTestFixture::ROM_PATH = "../bin/Debug/Demos";
//const std::string EmulatorTestFixture::ROM_PATH = "../bin/Debug/Games";
#else
//const std::string EmulatorTestFixture::ROM_PATH = "../bin/Release/Tests";
#endif

#endif // EMULATOR_TESTS_H
