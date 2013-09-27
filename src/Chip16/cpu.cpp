#include "cpu.h"

#include <functional>

CPU::CPU() { }

CPU::~CPU() { }

// TODO : Finish it and ask yourself what could go wrong
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

void CPU::InterpretOp()
{
	// The magic starts here
	switch (m_Memory[m_PC] >> 4)
	{
		case 0x0:	// Misc/dispatch to the other processing units mostly
		{
			InterpretMisc();
			break;
		}
		case 0x1:	// Call/Jump
		{
			InterpretCallJumps();
			break;
		}
		case 0x2:	// Load
		{
			InterpretLoads();
			break;
		}
		case 0x3:	// Store
		{
			InterpretStores();
			break;
		}
		case 0x4:	// Add
		{
			InterpretArithmetics(std::plus<UInt16>(), AddTag());
			break;
		}
		case 0x5:	// Sub
		{
			InterpretArithmetics(std::minus<UInt16>(), SubTag());
			break;
		}
		case 0x6:	// And
		{
			InterpretArithmetics(std::bit_and<UInt16>(), Tag());
			break;
		}
		case 0x7:	// Or
		{
			InterpretArithmetics(std::bit_or<UInt16>(), Tag());
			break;
		}
		case 0x8:	// Xor
		{
			InterpretArithmetics(std::bit_xor<UInt16>(), Tag());
			break;
		}
		case 0x9:	// Mul
		{
			InterpretArithmetics(std::multiplies<UInt16>(), MulTag());
			break;
		}
		case 0xA:	// Div
		{
			InterpretArithmetics(std::divides<UInt16>(), DivTag());
			break;
		}
		case 0xB:	// Shift
		{
			InterpretShifts();
			break;
		}
		case 0xC:	// Push/Pop
		{
			InterpretPushPops();
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

void CPU::InterpretCallJumps()
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
			// TODO : Private Push/Pop functions dealing with the emulator's stack
			m_PC++;
			UInt16 l_Addr = FetchImmediateValue();
			m_Memory[m_SP++] = m_PC & 0xF0;
			m_Memory[m_SP++] = m_PC & 0xF;
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

bool CPU::InterpretConditions(UInt8 in_CondCode)
{
	switch (in_CondCode & 0xF)
	{
		case 0x0:	// Z
			return m_FR & 0x4;
		case 0x1:	// NZ
			return !(m_FR & 0x4);
		case 0x2:	// N
			return m_FR & 0x80;
		case 0x3:	// NN
			return !(m_FR & 0x80);
		case 0x4:	// P
			return !(m_FR & 0x4) && !(m_FR & 0x80);
		case 0x5:	// O
		case 0x9:	// B
			return m_FR & 0x40;
		case 0x6:	// NO
		case 0x8:	// AE
			return !(m_FR & 0x40);
		case 0x7:	// A
			return !(m_FR & 0x4) && !(m_FR & 0x40);
		case 0xA:	// BE
			return (m_FR & 0x4) || (m_FR & 0x40);
		case 0xB:	// G
			return (m_FR & 0x4) && ((m_FR & 0x40) == (m_FR & 0x80));
		case 0xC:	// GE
			return ((m_FR & 0x40) == (m_FR & 0x80));
		case 0xD:	// L
			return ((m_FR & 0x40) != (m_FR & 0x80));
		case 0xE:	// LE
			return m_FR & 0x4 || ((m_FR & 0x40) != (m_FR & 0x80));
		default:
		{
			// PANIC !!!
			// TODO : Panic behavior
			break;
		}
	}
}

void CPU::InterpretLoads()
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// LDI	(register)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			UInt16 l_IVal = FetchImmediateValue();
			m_Registers[l_Addr] = l_IVal;
			break;
		}
		case 0x1:	// LDI	(stack pointer)
		{
			// TODO : Check for stack overflow
			m_PC++;
			UInt16 l_IVal = FetchImmediateValue();
			m_SP = l_IVal;
			break;
		}
		case 0x2:	// LDM	(direct)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			UInt16 l_IVal = FetchImmediateValue();
			m_Registers[l_Addr] = m_Memory[l_IVal];
			break;
		}
		case 0x3:	// LDM	(indirect)
		{
			UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
			UInt16 l_AddrY = m_Memory[m_PC] & 0xF0;
			UInt16 l_Val = ((0 | m_Memory[l_AddrY]) << 8) | m_Memory[l_AddrY+1];
			m_Registers[l_AddrX] = l_Val;
			m_PC += 3;
			break;
		}
		case 0x4:	// MOV
		{
			UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
			UInt16 l_AddrY = m_Memory[m_PC] & 0xF0;
			m_Registers[l_AddrX] = m_Registers[l_AddrY];
			m_PC += 3;
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

void CPU::InterpretMisc()
{
	switch (in_OpCode >> 24 & 0xF)
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
		case 0x2:	// VBLNK
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
		case 0x7:	// RND
		{
			// TODO
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

void CPU::InterpretPushPops()
{
	// TODO : Check for stack overflow
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// PUSH
		{
			m_Memory[m_SP++] = m_Registers[m_Memory[m_PC]] & 0xF0;
			m_Memory[m_SP++] = m_Registers[m_Memory[m_PC]] & 0xF;
			m_PC += 3;
			break;
		}
		case 0x1:	// POP
		{
			m_Registers[m_Memory[m_PC]] = m_Memory[m_SP--];
			m_Registers[m_Memory[m_PC]] << 8;
			m_Registers[m_Memory[m_PC]] |= m_Memory[m_SP--];
			break;
		}
		case 0x2:	// PUSHALL
		{
			for(UInt16 i = 0; i < 16; ++i)
			{
				m_Memory[m_SP++] = m_Registers[i] & 0xF0;
				m_Memory[m_SP++] = m_Registers[i] & 0xF;
			}
			break;
		}
		case 0x3:	// POPALL
		{
			for(UInt16 i = 15; i > -1; --i)
			{
				m_Registers[i] = m_Memory[m_SP--];
				m_Registers[i] << 8;
				m_Registers[i] |= m_Memory[m_SP--];
			}
			break;
		}
		case 0x4:	// PUSHF
		{
			m_Memory[m_SP] = m_FR;
			m_SP += 2;
			break;
		}
		case 0x5:	// POPF
		{
			m_FR = m_Memory[m_SP];
			m_SP -= 2;
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

void CPU::InterpretShifts()
{
	unsigned l_ShiftValue = 0;

	if(in_OpCode >> 20 & 0xF)	// Number of times to shift is stored in a register
		l_ShiftValue = m_Registers[in_OpCode >> 20 & 0xF];
	else
		l_ShiftValue = in_OpCode >> 8 & 0xF;

	
	switch (in_OpCode >> 24 & 0xF)
	{
		case 0x0:	// SHL
		{
			
			break;
		}
		case 0x1:	// SHR
		{
			// TODO
			break;
		}
		case 0x2:	// SAL
		{
			// TODO
			break;
		}
		case 0x3:	// SAR
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

void CPU::InterpretStores()
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// STM	(direct)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Memory[FetchImmediateValue()] = m_Registers[l_Addr];
			break;
		}
		case 0x1:	// STM	(indirect)
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			m_Memory[l_YVal] = l_XVal;
			m_PC += 2;
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

UInt16 CPU::FetchImmediateValue()
{
	UInt16 l_IVal = m_Memory[m_PC++];
	l_IVal >>= 8;
	return l_IVal | m_Memory[m_PC++];
}

UInt16 CPU::FetchRegisterAddress()
{
	return m_Memory[m_PC++] & 0xF;
}

void CPU::FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y)
{
	out_X = m_Registers[m_Memory[m_PC] & 0xF];
	out_Y = m_Registers[m_Memory[m_PC] & 0xF0];
}

void CPU::SetSignZeroFlag(UInt16 in_Result)
{
	// Set the zero flag (Bit[2])
	m_FR = in_Result == 0 ? m_FR | ZeroFlag : m_FR & ~ZeroFlag;
	// Set the negative flag (Bit[7])
	m_FR = in_Result & 0x4000 ? m_FR | NegativeFlag : m_FR & ~NegativeFlag;
}

// TODO
void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, Tag) { }
void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, AddTag) { }
void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, DivTag) { }
void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, MulTag) { }
void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, SubTag) { }
