#include "cpu.h"

CPU::CPU() { }

CPU::~CPU() 
{
	RegisterOpCode<0x00, ArithmeticInstruction<std::plus, ImmediateOpMode>>();
}

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

void  CPU::Run() 
{
	m_FctTable[FetchOpCode()](); 
}

UInt32 CPU::FetchOpCode()
{
	return (m_Memory[m_PC] >> 8 | m_Memory[m_PC+=1]);
}

UInt16 CPU::GetInputRegisterValue()
{
	return m_Registers[m_Memory[m_PC++] >> 24];
}
