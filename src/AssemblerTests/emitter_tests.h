#ifndef EMITTER_TESTS_H
#define EMITTER_TESTS_H

#include "emitter.h"

/**
* \struct EmitterFixture
* \brief Fixture containing data to test the emitter functions.
* \author Felix-Antoine Ouellet
*/
struct EmitterFixture
{
	const char const* TestFilename = "test.c16";
	
	Emitter Emitter;

	EmitterFixture() { }
};

#endif // EMITTER_TESTS_H
