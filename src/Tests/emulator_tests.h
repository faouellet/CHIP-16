#ifndef EMULATOR_TESTS_H
#define EMULATOR_TESTS_H

#include <string>

#include "emulator.h"

struct EmulatorFixture
{
	static const std::string ROM_PATH;

	Emulator Emu;

	EmulatorFixture() { }
};

const std::string ROM_PATH = "../../Chip16 Program Pack";

#endif // EMULATOR_TESTS_H
