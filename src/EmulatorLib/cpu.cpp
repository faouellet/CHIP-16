#include "cpu.h"

#include <iostream>
#include <iterator>
#include <limits>

CPU::CPU() : m_Dist(0U, std::numeric_limits<UInt16>::max()), m_FR(0), m_PC(0), m_SP(0), m_ErrorCode(0)
{
	for(int i = 0; i < 16; ++i)
		m_Registers[i] = 0;

	for(int i = 0; i < MEMORY_SIZE; ++i)
		m_Memory[i] = 0;
}

CPU::~CPU() { }

std::vector<UInt8> CPU::DumpMemory() const
{
	return std::vector<UInt8>(std::begin(m_Memory), std::end(m_Memory));
}

UInt16 CPU::DumpProgramCounter() const
{
	return m_PC;
}

std::vector<UInt16> CPU::DumpRegisters() const
{
	return std::vector<UInt16>(std::begin(m_Registers), std::end(m_Registers));
}

UInt16 CPU::DumpStackPointer() const
{
	return m_SP;
}

// TODO : Ask yourself what could go wrong
unsigned CPU::Init(std::vector<UInt8> && in_ROMData) 
{
	if(in_ROMData.empty())
		return EmptyROMError;
	if(in_ROMData.size() > STACK_START)
		return ROMOverflowError;

	for(int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = in_ROMData[i];

	for(unsigned i = HEADER_SIZE ; i < in_ROMData.size(); ++i)
		m_Memory[i-HEADER_SIZE] = in_ROMData[i];

	m_PC = m_ROMHeader[0xA];
	m_SP = STACK_START;

	unsigned l_Ret = NoError;
	
	if(!m_GPU.Init())
		l_Ret |= GPUError;
	if(!m_SPU.Init())
		l_Ret |= SPUError;

	return l_Ret;
}

void CPU::Reset()
{
	for(int i = 0; i < HEADER_SIZE; ++i)
		m_ROMHeader[i] = 0;

	for(int i = 0; i < MEMORY_SIZE; ++i)
		m_Memory[i] = 0;

	m_ErrorCode = 0;
	m_FR = 0;
	m_SP = 0;
	m_PC = 0;
}

unsigned CPU::InterpretOp()
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
			InterpretArithmetics(std::plus<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagAdd(in_Op1, in_Op2); });
			break;
		}
		case 0x5:	// Sub
		{
			InterpretArithmetics(std::minus<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
			break;
		}
		case 0x6:	// And
		{
			InterpretArithmetics(std::bit_and<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); });
			break;
		}
		case 0x7:	// Or
		{
			InterpretArithmetics(std::bit_or<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); });
			break;
		}
		case 0x8:	// Xor
		{
			InterpretArithmetics(std::bit_xor<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); });
			break;
		}
		case 0x9:	// Mul
		{
			InterpretArithmetics(std::multiplies<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagMul(in_Op1, in_Op2); });
			break;
		}
		case 0xA:	// Div
		{
			InterpretArithmetics(std::divides<UInt16>(), [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
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
		case 0xD:	// Palette
		{
			InterpretPalettes();
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[m_PC]);
			m_ErrorCode |= UnknownError;
			break;
		}
	}
	if(m_PC > m_ROMHeader[6])	// TODO : make sure with the test ROMs
		m_ErrorCode |= EmulationDone;
	return m_ErrorCode;
}

void CPU::InterpretArithmetics(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// X = F(X, I)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			UInt16 l_IVal = FetchImmediateValue();
			in_FRH(l_Addr, l_IVal);
			m_Registers[l_Addr] = in_Ins(m_Registers[l_Addr], l_IVal);
			SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
			break;
		}
		case 0x1:	// X = F(X, Y)
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			in_FRH(l_XVal, l_YVal);
			m_Registers[m_Memory[m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
			SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
			m_PC += 3;
			break;
		}
		case 0x2:	// Z = F(X, Y)
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			in_FRH(l_XVal, l_YVal);
			m_Registers[m_Memory[++m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
			SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
			m_PC += 2;
			break;
		}
		case 0x3:	// F(X, I)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			UInt16 l_IVal = FetchImmediateValue();
			UInt16 l_Result = in_Ins(m_Registers[l_Addr], l_IVal);
			SetSignZeroFlag(l_Result);
			break;
		}
		case 0x4:	// F(X, Y)
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			UInt16 l_Result = in_Ins(l_XVal, l_YVal);
			SetSignZeroFlag(l_Result);
			m_PC += 3;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= ArithmeticError;
			break;
		}
	}
}

void CPU::InterpretCallJumps()
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// JMP	(direct)
		{
			m_PC++;
			m_PC = FetchImmediateValue();
			break;
		}
		case 0x2:	// Jx
		{
			UInt8 l_CondCode = m_Memory[m_PC++];
			if(InterpretConditions(l_CondCode))
				m_PC = FetchImmediateValue();
			else
				if(!(m_ErrorCode & ConditionError))
					m_PC += 2;
			break;
		}
		case 0x3:	// JME
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			m_PC++;
			if(l_XVal == l_YVal)
				m_PC = FetchImmediateValue();
			else
				m_PC += 2;
			break;
		}
		case 0x4:	// CALL	(direct)
		{
			m_PC++;
			UInt16 l_Addr = FetchImmediateValue();
			Push(m_PC);
			m_PC = l_Addr;
			break;
		}
		case 0x5:	// RET
		{
			m_PC = Pop();
			break;
		}
		case 0x6:	// JMP	(indirect)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_PC = m_Registers[l_Addr];
			break;
		}
		case 0x7:	// Cx
		{
			UInt8 l_CondCode = m_Memory[m_PC++];
			if(InterpretConditions(l_CondCode))
			{
				UInt16 l_Addr = FetchImmediateValue();
				Push(m_PC);
				m_PC = l_Addr;
			}
			else
			{
				if(!(m_ErrorCode & ConditionError))
					m_PC += 2;
			}
			break;
		}
		case 0x8:	// CALL	(indirect)
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_PC += 3;
			Push(m_PC);
			m_PC = m_Registers[m_PC];
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= CallJumpError;
			break;
		}
	}
}

unsigned CPU::InterpretConditions(UInt8 in_CondCode)
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
			m_ErrorCode |= ConditionError;
			return 0;
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
			m_Registers[l_Addr] = (m_Memory[l_IVal] << 8) | m_Memory[l_IVal+1];
			break;
		}
		case 0x3:	// LDM	(indirect)
		{
			UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
			UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
			UInt16 l_Val = (m_Memory[m_Registers[l_AddrY]] << 8) | m_Memory[m_Registers[l_AddrY]+1];
			m_Registers[l_AddrX] = l_Val;
			m_PC += 3;
			break;
		}
		case 0x4:	// MOV
		{
			UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
			UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
			m_Registers[l_AddrX] = m_Registers[l_AddrY];
			m_PC += 3;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= MemoryError;
			break;
		}
	}
}

void CPU::InterpretMisc()
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// NOP	
		{
			break;
		}
		case 0x1:	// CLS
		{
			m_GPU.ClearScreen();
			m_PC += 3;
			break;
		}
		case 0x2:	// VBLNK
		{
			if(!m_GPU.VBlankFlag())
				m_PC--;
			break;
		}
		case 0x3:	// BGC
		{
			m_PC++;
			m_GPU.SetBackgroundColor(m_Memory[m_PC++] & 0xF);
			m_PC++;
			break;
		}
		case 0x4:	// SPR
		{
			m_PC++;
			m_GPU.SetSpriteDimensions(m_Memory[m_PC++], m_Memory[m_PC++]);
			break;
		}
		case 0x5:	// DRW
		{
			// TODO : Deal with carry flag
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			m_PC++;
			UInt16 l_Addr = FetchImmediateValue();
			m_GPU.Draw(l_XVal, l_YVal, m_Memory[l_Addr]);
			break;
		}
		case 0x6:	// DRW
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			m_PC++;
			UInt16 l_Addr = m_Memory[m_PC++];
			m_PC++;
			m_GPU.Draw(l_XVal, l_YVal, m_Memory[l_Addr]);
			break;
		}
		case 0x7:	// RND
		{
			UInt16 l_Addr = FetchRegisterAddress();
			UInt16 l_MaxVal = FetchImmediateValue();
			UInt16 l_RandVal = m_Dist(m_RandEngine);
			if(l_RandVal > l_MaxVal)
				l_RandVal -= l_MaxVal;
			m_Registers[l_Addr] = l_RandVal;
			break;
		}
		case 0x8:	// FLIP
		{
			m_PC += 2;
			m_GPU.Flip(m_Memory[m_PC], m_Memory[m_PC++]);
			break;
		}
		case 0x9:	// SND0
		{
			m_PC += 3;
			m_SPU.Stop();
			break;
		}
		case 0xA:	// SND1
		{
			m_PC++;
			m_SPU.PlayTone(500, FetchImmediateValue());
			break;
		}
		case 0xB:	// SND2
		{
			m_PC++;
			m_SPU.PlayTone(1000, FetchImmediateValue());
			break;
		}
		case 0xC:	// SND3
		{
			m_PC++;
			m_SPU.PlayTone(1500, FetchImmediateValue());
			break;
		}
		case 0xD:	// SNP
		{
			m_SPU.PlayTone(FetchRegisterAddress(), FetchImmediateValue());
			break;
		}
		case 0xE:	// SNG
		{
			m_SPU.GenerateSound((m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF, 
				(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF,
				(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF);
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= DeviceError;
			break;
		}
	}
}

void CPU::InterpretPalettes()
{
	UInt8 l_PaletteData[16][3];	// Per specifications
	UInt16 l_Addr;
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:
		{
			m_PC++;
			l_Addr = FetchImmediateValue();
			break;
		}
		case 0x1:
		{
			l_Addr = FetchRegisterAddress();
			m_PC += 3;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= PaletteError;
			return;
		}
	}

	for(UInt16 i = 0; i < 16; ++i)
	{
		l_PaletteData[i][0] = m_Memory[l_Addr+i];
		l_PaletteData[i][1] = m_Memory[l_Addr+i+1];
		l_PaletteData[i][2] = m_Memory[l_Addr+i+2];
	}
	m_GPU.LoadPalette(l_PaletteData);
}

void CPU::InterpretPushPops()
{
	// TODO : Check for stack overflow
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// PUSH
		{
			Push(m_Registers[m_Memory[m_PC]]);
			m_PC += 3;
			break;
		}
		case 0x1:	// POP
		{
			m_Registers[m_Memory[m_PC]] = Pop();
			m_PC += 3;
			break;
		}
		case 0x2:	// PUSHALL
		{
			for(UInt16 i = 0; i < 16; ++i)
				Push(m_Registers[i]);
			m_PC += 3;
			break;
		}
		case 0x3:	// POPALL
		{
			for(Int16 i = 15; i > -1; --i)
				m_Registers[i] = Pop();
			m_PC += 3;
			break;
		}
		case 0x4:	// PUSHF
		{
			m_Memory[m_SP] = m_FR;
			m_SP += 2;
			m_PC += 3;
			break;
		}
		case 0x5:	// POPF
		{
			m_FR = m_Memory[m_SP];
			m_SP -= 2;
			m_PC += 3;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= StackError;
			break;
		}
	}
}

void CPU::InterpretShifts()
{
	switch (m_Memory[m_PC++] & 0xF)
	{
		case 0x0:	// SHL, SAL
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
			m_PC++;
			break;
		}
		case 0x1:	// SHR
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
			m_PC++;
			break;
		}
		case 0x2:	// SAR
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
			m_PC++;
			break;
		}
		case 0x3:	// SHL
		{
			UInt16 l_Addr = m_Memory[m_PC] & 0xF;
			m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
			m_PC += 2;
			break;
		}
		case 0x4:	// SHR
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
			m_PC++;
			break;
		}
		case 0x5:	// SAR
		{
			UInt16 l_Addr = FetchRegisterAddress();
			m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
			m_PC++;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= ArithmeticError;
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
			UInt16 l_RegAddr = FetchRegisterAddress();
			UInt16 l_MemAddr = FetchImmediateValue();
			m_Memory[l_MemAddr] = m_Registers[l_RegAddr] & 0xFF00;
			m_Memory[l_MemAddr+1] = m_Registers[l_RegAddr] & 0x00FF;
			break;
		}
		case 0x1:	// STM	(indirect)
		{
			UInt16 l_XVal, l_YVal;
			FetchRegistersValues(l_XVal, l_YVal);
			m_Memory[m_Registers[l_YVal]] = l_XVal & 0xFF00;
			m_Memory[m_Registers[l_YVal]+1] = l_XVal & 0x00FF;
			m_PC += 3;
			break;
		}
		default:
		{
			OutputUnknownOpCode(m_Memory[--m_PC]);
			m_ErrorCode |= MemoryError;
			break;
		}
	}
}

UInt16 CPU::FetchImmediateValue()
{
	UInt16 l_IVal = m_Memory[m_PC++];
	return l_IVal | (m_Memory[m_PC++] << 8);
}

UInt16 CPU::FetchRegisterAddress()
{
	return m_Memory[m_PC++] & 0xF;
}

void CPU::FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y)
{
	out_X = m_Registers[m_Memory[m_PC] & 0xF];
	out_Y = m_Registers[(m_Memory[m_PC] & 0xF0) >> 4];
}

void CPU::OutputUnknownOpCode(UInt8 in_Code)
{
	std::cout << "Unknown opcode: " << std::hex << in_Code << std::endl;
}

UInt16 CPU::Pop()
{
	return UInt16(m_Memory[--m_SP] | (m_Memory[--m_SP] << 8));
}

void CPU::Push(UInt16 in_Val)
{
	m_Memory[m_SP++] = (in_Val & 0xFF00) >> 8;
	m_Memory[m_SP++] = in_Val & 0x00FF;
}

void CPU::SetSignZeroFlag(UInt16 in_Result)
{
	// Set the zero flag (Bit[2])
	m_FR = in_Result == 0 ? m_FR | ZeroFlag : m_FR & ~ZeroFlag;
	// Set the negative flag (Bit[7])
	m_FR = in_Result & 0x4000 ? m_FR | NegativeFlag : m_FR & ~NegativeFlag;
}

void CPU::SetCarryOverflowFlagAdd(UInt16 in_Op1, UInt16 in_Op2) 
{
	UInt16 l_Result = in_Op1 + in_Op2;
	// Set carry flag
	m_FR = l_Result < in_Op1 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 < 0 && in_Op2 < 0)
		|| (l_Result < 0 && in_Op1 >= 0 && in_Op2 >= 0) ?
		m_FR | SignedOverflowFlag : m_FR & ~SignedOverflowFlag;
}

void CPU::SetCarryOverflowFlagDiv(UInt16 in_Op1, UInt16 in_Op2) 
{
	// Set carry flag
	m_FR = in_Op1 % in_Op2 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::SetCarryOverflowFlagMul(UInt16 in_Op1, UInt16 in_Op2) 
{
	// Set carry flag
	UInt32 l_Result = in_Op1 * in_Op2;
	m_FR = l_Result > std::numeric_limits<UInt16>::max() ? 
		m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::SetCarryOverflowFlagSub(UInt16 in_Op1, UInt16 in_Op2) 
{
	UInt16 l_Result = in_Op1 - in_Op2;
	// Set carry flag
	m_FR = l_Result > in_Op1 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 < 0 && in_Op2 >= 0)
		|| (l_Result < 0 && in_Op1 >= 0 && in_Op2 < 0) ?
		m_FR | SignedOverflowFlag : m_FR & ~SignedOverflowFlag;
}
