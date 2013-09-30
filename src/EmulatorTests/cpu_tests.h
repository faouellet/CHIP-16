#ifndef CPU_TESTS_H
#define CPU_TESTS_H

#include "cpu.h"
#include "cpu.cpp"

/*
* \struct CPUFixture
* \brief Fixture containing data to test the CPU specific instructions.
*        That means the arithmetic, call, jump, load, pop, push, shift and
*         store instructions.
*/
struct CPUFixture
{
	CPU cpu;

	CPUFixture() { }
};

#endif // CPU_TESTS_H
