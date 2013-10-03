#ifndef EMULATOR_TESTS_H
#define EMULATOR_TESTS_H

#include <string>

#include "emulator.h"

struct EmulatorTestFixture
{
	static const std::string ROM_PATH;
	
	Emulator Emu;

	EmulatorTestFixture() { }
};

const std::string EmulatorTestFixture::ROM_PATH = "../../Chip16 Program Pack/Tests";

#endif // EMULATOR_TESTS_H
