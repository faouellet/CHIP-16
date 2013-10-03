#ifndef EMULATOR_DEMOS_H
#define EMULATOR_DEMOS_H

#include <string>

#include "emulator.h"

struct EmulatorDemoFixture
{
	static const std::string ROM_PATH;
	
	Emulator Emu;

	EmulatorDemoFixture() { }
};

const std::string ROM_PATH = "../../Chip16 Program Pack/Demos";

#endif // EMULATOR_DEMOS_H
