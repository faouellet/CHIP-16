#include "cpu.h"

CPU::CPU() { }

CPU::~CPU() { }

// TODO : Finish it and ask yourself what could go wrong
bool CPU::Init(std::vector<const UInt8> && in_ROMData) 
{
	for(int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = in_ROMData[i];

	for(unsigned i = HEADER_SIZE ; i < in_ROMData.size(); ++i)
		m_Memory[i-HEADER_SIZE] = in_ROMData[i];

	return true;
}

// Q : Could it be possible to use template or macro magic to automatically generate some interpretations ? 
void CPU::InterpretOp()
{
	UInt32 l_OpCode = FetchOpCode();

	// The magic starts here
	switch (l_OpCode >> 24 & 0x3)
	{
		case 0x0:
		{
			InterpretMisc(std::move(l_OpCode));
		}
		case 0x1:
		{
			InterpretJumps(std::move(l_OpCode));
			// InterpretCalls ??
		}
		case 0x2:
		{
			InterpretLoads(std::move(l_OpCode));
		}
		case 0x3:
		{
			InterpretStores(std::move(l_OpCode));
		}
		case 0x4:	// ADD
		case 0x5:	// SUB
		case 0x6:	// AND
		case 0x7:	// OR
		case 0x8:	// XOR
		case 0x9:	// MUL
		case 0xA:	// DIV
		case 0xB:	// Shift
		{
			InterpretArithmetics(std::move(l_OpCode));
		}
		case 0xC:
		{
			InterpretPushPops(std::move(l_OpCode));
		}
		default:
		{
			break;
		}
	}
}

void CPU::InterpretArithmetics(UInt32 && in_OpCode)
{
	// TODO
}

void CPU::InterpretCalls(UInt32 && in_OpCode)
{
	// TODO
}

void CPU::InterpretJumps(UInt32 && in_OpCode)
{
	// TODO
}

void CPU::InterpretLoads(UInt32 && in_OpCode)
{
	// TODO
}

void CPU::InterpretMisc(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24)
	{
		case 0x0:	// NOP	
		{
			break;
		}
		case 0x1:	// CLS
		{
			// TODO
			break;
		}
		case 0x2:	//VBLNK
		{
			// TODO
			break;
		}
		case 0x3:	// BGC
		{
			// TODO : Dispatch to GPU
			break;
		}
		case 0x4:	// SPR
		{
			// TODO : Dispatch to GPU
			break;
		}
		case 0x5:
		case 0x6:	// DRW
		{
			// TODO : Dispatch to GPU
			break;
		}
		case 0x8:	// FLIP
		{
			// TODO : Dispatch to GPU
			break;
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretPushPops(UInt32 && in_OpCode)
{
	// TODO
}

void CPU::InterpretStores(UInt32 && in_OpCode)
{
	// TODO
}

UInt32 CPU::FetchOpCode()
{
	// Q : Should the last 2 bytes be read now or during a specific interpret function ?
	UInt32 l_OpCode = 0;
	for(int i = 0; i < OPCODE_SIZE; ++i)
		(l_OpCode << 8) | (m_Memory[m_PC+=i]);

	return l_OpCode;
}
