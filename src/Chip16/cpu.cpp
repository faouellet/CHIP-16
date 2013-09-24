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
	switch (l_OpCode >> 28 & 0xF)
	{
		case 0x0:	// Misc/dispatch to the other processing units mostly
		{
			InterpretMisc(std::move(l_OpCode));
		}
		case 0x1:	// Call/Jump
		{
			InterpretCallJumps(std::move(l_OpCode));
		}
		case 0x2:	// Load
		{
			InterpretLoads(std::move(l_OpCode));
		}
		case 0x3:	// Store
		{
			InterpretStores(std::move(l_OpCode));
		}
		case 0x4:	// Add
		{
			InterpretAdds(std::move(l_OpCode));
		}
		case 0x5:	// Sub
		{
			InterpretSubs(std::move(l_OpCode));
		}
		case 0x6:	// And
		{
			InterpretAnds(std::move(l_OpCode));
		}
		case 0x7:	// Or
		{
			InterpretOrs(std::move(l_OpCode));
		}
		case 0x8:	// Xor
		{
			InterpretXors(std::move(l_OpCode));
		}
		case 0x9:	// Mul
		{
			InterpretMuls(std::move(l_OpCode));
		}
		case 0xA:	// Div
		{
			InterpretDivs(std::move(l_OpCode));
		}
		case 0xB:	// Shift
		{
			InterpretShifts(std::move(l_OpCode));
		}
		case 0xC:	// Push/Pop
		{
			InterpretPushPops(std::move(l_OpCode));
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretAdds(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// ADDI
		{
			// TODO
		}
		case 0x1:	// ADD	(inplace)
		{
			// TODO
		}
		case 0x2:	// ADD
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretAnds(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// ANDI
		{
			// TODO
		}
		case 0x1:	// AND	(inplace)
		{
			// TODO
		}
		case 0x2:	// AND
		{
			// TODO
		}
		case 0x3:	// TSTI
		{
			// TODO
		}
		case 0x4:	// TST
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretCallJumps(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// JMP	(direct)
		{
			// TODO
			break;
		}
		case 0x2:	// Jx
		{
			// TODO
			break;
		}
		case 0x3:	// JME
		{
			// TODO
			break;
		}
		case 0x4:	// CALL	(direct)
		{
			// TODO
			break;
		}
		case 0x5:	// RET
		{
			// TODO
			break;
		}
		case 0x6:	// JMP	(indirect)
		{
			// TODO
			break;
		}
		case 0x7:	// Cx
		{
			// TODO
			break;
		}
		case 0x8:	// CALL	(indirect)
		{
			// TODO
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

void CPU::InterpretDivs(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// DIVI
		{
			// TODO
		}
		case 0x1:	// DIV	(inplace)
		{
			// TODO
		}
		case 0x2:	// DIV
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretLoads(UInt32 && in_OpCode)
{
	switch (in_OpCode  >> 24 & 0XF)
	{
		case 0x0:	// LDI	(register)
		{
			// TODO
			break;
		}
		case 0x1:	// LDI	(stack pointer)
		{
			// TODO
			break;
		}
		case 0x2:	// LDM	(direct)
		{
			// TODO
			break;
		}
		case 0x3:	// LDM	(indirect)
		{
			// TODO
			break;
		}
		case 0x4:	// MOV
		{
			// TODO
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

void CPU::InterpretMisc(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// NOP	
		{
			// TODO
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
		case 0x9:	// SND0
		{
			// TODO : Dispatch to SPU
			break;
		}
		case 0xA:	// SND1
		{
			// TODO : Dispatch to SPU
			break;
		}
		case 0xB:	// SND2
		{
			// TODO : Dispatch to SPU
			break;
		}
		case 0xC:	// SND3
		{
			// TODO : Dispatch to SPU
			break;
		}
		case 0xD:	// SNP
		{
			// TODO : Dispatch to SPU
			break;
		}
		case 0xE:	// SNG
		{
			// TODO : Dispatch to SPU
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

void CPU::InterpretMuls(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// MULI
		{
			// TODO
		}
		case 0x1:	// MUL	(inplace)
		{
			// TODO
		}
		case 0x2:	// MUL
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretOrs(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// ORI
		{
			// TODO
		}
		case 0x1:	// OR	(inplace)
		{
			// TODO
		}
		case 0x2:	// OR
		{
			// TODO
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
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// PUSH
		{
			// TODO
		}
		case 0x1:	// POP
		{
			// TODO
		}
		case 0x2:	// PUSHALL
		{
			// TODO
		}
		case 0x3:	// POPALL
		{
			// TODO
		}
		case 0x4:	// PUSHF
		{
			// TODO
		}
		case 0x5:	// POPF
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretShifts(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// SHL
		{
			// TODO
		}
		case 0x1:	// SHR
		{
			// TODO
		}
		case 0x2:	// SAL
		{
			// TODO
		}
		case 0x3:	// SAR
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretStores(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// STM	(direct)
		{
			// TODO
		}
		case 0x1:	// STM	(indirect)
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretSubs(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// SUBI
		{
			// TODO
		}
		case 0x1:	// SUB	(inplace)
		{
			// TODO
		}
		case 0x2:	// SUB
		{
			// TODO
		}
		case 0x3:	// CMPI
		{
			// TODO
		}
		case 0x4:	// CMP
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretXors(UInt32 && in_OpCode)
{
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// XORI
		{
			// TODO
		}
		case 0x1:	// XOR	(inplace)
		{
			// TODO
		}
		case 0x2:	// XOR
		{
			// TODO
		}
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

UInt32 CPU::FetchOpCode()
{
	// Q : Should the last 2 bytes be read now or during a specific interpret function ?
	UInt32 l_OpCode = 0;
	for(int i = 0; i < OPCODE_SIZE; ++i)
		l_OpCode = (l_OpCode << 8) | (m_Memory[m_PC+=i]);

	return l_OpCode;
}
