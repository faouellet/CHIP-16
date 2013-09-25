#include "TestNewCPU.h"

CPU::CPU() { }

CPU::~CPU() { }

bool CPU::Init(std::vector<UInt8> && in_ROMData) 
{
	for(int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = in_ROMData[i];

	for(unsigned i = HEADER_SIZE ; i < in_ROMData.size(); ++i)
		m_Memory[i-HEADER_SIZE] = in_ROMData[i];

	m_PC = m_ROMHeader[0xA];
	m_SP = 0xFDF0;

	return true;
}

void  CPU::InterpretOp() 
{
	m_FctTable[FetchOpCode()](); 
}

UInt32 CPU::FetchOpCode()
{
	// Q : Should the last 2 bytes be read now or during a specific interpret function ?
	UInt32 l_OpCode = 0;
	for(int i = 0; i < OPCODE_SIZE; ++i)
		l_OpCode = (l_OpCode << 8) | (m_Memory[m_PC+=i]);

	return l_OpCode;
}
