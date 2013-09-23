#include "cpu.h"

CPU::CPU() { }

CPU::~CPU() { }

bool CPU::Init(const unsigned char in_PCStart) 
{
	m_PC = in_PCStart;
}

void CPU::InterpretOp()
{

}
