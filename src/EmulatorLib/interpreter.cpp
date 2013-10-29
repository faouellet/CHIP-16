#include "interpreter.h"

#include <iostream>

Interpreter::Interpreter() : m_Dist(0, std::numeric_limits<UInt16>::max())
{
	m_Ops[0x00] = &Interpreter::NOP; 
	m_Ops[0x01] = &Interpreter::CLS;
	m_Ops[0x02] = &Interpreter::VBLNK; 
	m_Ops[0x03] = &Interpreter::BGC; 
	m_Ops[0x04] = &Interpreter::SPR; 
	m_Ops[0x05] = &Interpreter::ImmediateDRW; 
	m_Ops[0x06] = &Interpreter::RegisterDRW; 
	m_Ops[0x07] = &Interpreter::RND;
	
	m_Ops[0x08] = &Interpreter::FLIP; 
	
	m_Ops[0x09] = &Interpreter::SND0; 
	m_Ops[0x0A] = &Interpreter::SND1;
	m_Ops[0x0B] = &Interpreter::SND2;
	m_Ops[0x0C] = &Interpreter::SND3;
	m_Ops[0x0D] = &Interpreter::SNP; 
	m_Ops[0x0E] = &Interpreter::SNG;

	m_Ops[0x10] = &Interpreter::DirectJMP; 
	m_Ops[0x12] = &Interpreter::Jx;
	m_Ops[0x13] = &Interpreter::JME; 
	m_Ops[0x14] = &Interpreter::DirectCALL; 
	m_Ops[0x15] = &Interpreter::RET; 
	m_Ops[0x16] = &Interpreter::IndirectJMP; 
	m_Ops[0x17] = &Interpreter::Cx; 
	m_Ops[0x18] = &Interpreter::IndirectCALL;

	m_Ops[0x20] = &Interpreter::RegisterLDI;
	m_Ops[0x21] = &Interpreter::StackLDI;
	m_Ops[0x22] = &Interpreter::DirectLDM;
	m_Ops[0x23] = &Interpreter::IndirectLDM; 
	m_Ops[0x24] = &Interpreter::MOV;

	m_Ops[0x30] = &Interpreter::DirectSTM; 
	m_Ops[0x31] = &Interpreter::IndirectSTM;

	m_Ops[0x40] = &Interpreter::ADDI;
	m_Ops[0x41] = &Interpreter::InplaceADD;
	m_Ops[0x42] = &Interpreter::ADD;

	m_Ops[0x50] = &Interpreter::SUBI;
	m_Ops[0x51] = &Interpreter::InplaceSUB;
	m_Ops[0x52] = &Interpreter::SUB;

	m_Ops[0x53] = &Interpreter::CMPI;
	m_Ops[0x54] = &Interpreter::CMP;

	m_Ops[0x60] = &Interpreter::ANDI;
	m_Ops[0x61] = &Interpreter::InplaceAND;
	m_Ops[0x62] = &Interpreter::AND;
	
	m_Ops[0x63] = &Interpreter::TSTI;
	m_Ops[0x64] = &Interpreter::TST;

	m_Ops[0x70] = &Interpreter::ORI;
	m_Ops[0x71] = &Interpreter::InplaceOR;
	m_Ops[0x72] = &Interpreter::OR;

	m_Ops[0x80] = &Interpreter::XORI;
	m_Ops[0x81] = &Interpreter::InplaceXOR;
	m_Ops[0x82] = &Interpreter::XOR;

	m_Ops[0x90] = &Interpreter::MULI;
	m_Ops[0x91] = &Interpreter::InplaceMUL;
	m_Ops[0x92] = &Interpreter::MUL;
	
	m_Ops[0xA0] = &Interpreter::DIVI;
	m_Ops[0xA1] = &Interpreter::InplaceDIV;
	m_Ops[0xA2] = &Interpreter::DIV;
	
	m_Ops[0xB0] = &Interpreter::NSHL;
	m_Ops[0xB1] = &Interpreter::NSHR;
	m_Ops[0xB2] = &Interpreter::NSAR;
	m_Ops[0xB3] = &Interpreter::RegisterSHL;
	m_Ops[0xB4] = &Interpreter::RegisterSHR;
	m_Ops[0xB5] = &Interpreter::RegisterSAR;

	m_Ops[0xC0] = &Interpreter::PUSH;
	m_Ops[0xC1] = &Interpreter::POP;
	m_Ops[0xC2] = &Interpreter::PUSHALL;
	m_Ops[0xC3] = &Interpreter::POPALL;
	m_Ops[0xC4] = &Interpreter::PUSHF;
	m_Ops[0xC5] = &Interpreter::POPF;

	m_Ops[0xD0] = &Interpreter::ImmediatePalette;
	m_Ops[0xD1] = &Interpreter::RegisterPalette;
}

Interpreter::~Interpreter() { }

unsigned Interpreter::InterpretOp()
{
	UInt32 l_Instruction = m_CPU.FetchInstruction();
	Instruction l_Func = m_Ops[l_Instruction >> 24];
	if(l_Func)
		(this->*l_Func)(l_Instruction);
	else
		std::cout << "Unknown opcode" << std::endl;
	
	return 0; // TODO : Error code
}

/////////////// Arithmetic ///////////////

void Interpreter::ADDI(const UInt32 in_Instruction) { ImmediateArithmetic(std::plus<UInt16>(),
										[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void Interpreter::InplaceADD(const UInt32 in_Instruction) { InplaceArithmetic(std::plus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void Interpreter::ADD(const UInt32 in_Instruction) { BasicArithmetic(std::plus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void Interpreter::SUBI(const UInt32 in_Instruction) { ImmediateArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void Interpreter::InplaceSUB(const UInt32 in_Instruction) { InplaceArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void Interpreter::SUB(const UInt32 in_Instruction) { BasicArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void Interpreter::CMPI(const UInt32 in_Instruction) { DiscardImmediateArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void Interpreter::CMP(const UInt32 in_Instruction) { DiscardArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void Interpreter::ANDI(const UInt32 in_Instruction) { ImmediateArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::InplaceAND(const UInt32 in_Instruction) { InplaceArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::AND(const UInt32 in_Instruction) { BasicArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::TSTI(const UInt32 in_Instruction) { DiscardImmediateArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::TST(const UInt32 in_Instruction) { DiscardArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::ORI(const UInt32 in_Instruction) { ImmediateArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::InplaceOR(const UInt32 in_Instruction) { InplaceArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::OR(const UInt32 in_Instruction) { BasicArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::XORI(const UInt32 in_Instruction) { ImmediateArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::InplaceXOR(const UInt32 in_Instruction) { InplaceArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::XOR(const UInt32 in_Instruction) { BasicArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void Interpreter::MULI(const UInt32 in_Instruction) { ImmediateArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void Interpreter::InplaceMUL(const UInt32 in_Instruction) { InplaceArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void Interpreter::MUL(const UInt32 in_Instruction) { BasicArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void Interpreter::DIVI(const UInt32 in_Instruction) { ImmediateArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

void Interpreter::InplaceDIV(const UInt32 in_Instruction) { InplaceArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

void Interpreter::DIV(const UInt32 in_Instruction) { BasicArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU.SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

/////////////// Call/Jump ///////////////

void Interpreter::DirectJMP()
{
	++m_PC;
	m_PC = FetchImmediateValue();
}

void Interpreter::Jx()
{
	UInt8 l_CondCode = m_Memory[m_PC++];
	if(InterpretConditions(l_CondCode))
		m_PC = FetchImmediateValue();
	else
		m_PC += 2;
}

void Interpreter::JME()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	if(l_XVal == l_YVal)
		m_PC = FetchImmediateValue();
	else
		m_PC += 2;
}

void Interpreter::DirectCALL()
{
	++m_PC;
	UInt16 l_Addr = FetchImmediateValue();
	Push(m_PC);
	m_PC = l_Addr;
}

void Interpreter::RET()
{
	m_PC = Pop();
}

void Interpreter::IndirectJMP()
{
	m_PC = m_Registers[FetchRegisterAddress()];
}

void Interpreter::Cx()
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
		m_PC += 2;
	}
}

void Interpreter::IndirectCALL()
{
	UInt16 l_Addr = FetchRegisterAddress();
    m_PC += 2;
    Push(m_PC);
    m_PC = m_Registers[l_Addr];
}

unsigned Interpreter::InterpretConditions(UInt8 in_CondCode)
{
	switch (in_CondCode & 0xF)
	{
		case 0x0:	// Z
			return m_FR & ZeroFlag;
		case 0x1:	// NZ
			return m_FR ^ (m_FR | ZeroFlag);
		case 0x2:	// N
			return m_FR & NegativeFlag;
		case 0x3:	// NN
			return (m_FR ^ (m_FR | NegativeFlag));
		case 0x4:	// P
			return !(m_FR & ZeroFlag) && (m_FR ^ (m_FR | NegativeFlag));
		case 0x5:	// O
			return m_FR & SignedOverflowFlag;
		case 0x6:	// NO
			return m_FR ^ (m_FR | SignedOverflowFlag);
		case 0x7:	// A
			return (m_FR ^ (m_FR | ZeroFlag)) && (m_FR ^ (m_FR | SignedOverflowFlag));
		case 0x8:	// AE
			return m_FR ^ (m_FR | UnsignedCarryFlag);
		case 0x9:	// B
			return m_FR & UnsignedCarryFlag;
		case 0xA:	// BE
			return (m_FR & ZeroFlag) || (m_FR & UnsignedCarryFlag);
		case 0xB:	// G
			return (m_FR & ZeroFlag) && ((m_FR & SignedOverflowFlag) == (m_FR & NegativeFlag));
		case 0xC:	// GE
			return ((m_FR & SignedOverflowFlag) == (m_FR & NegativeFlag));
		case 0xD:	// L
			return ((m_FR & SignedOverflowFlag) != (m_FR & NegativeFlag));
		case 0xE:	// LE
			return m_FR & ZeroFlag || ((m_FR & SignedOverflowFlag) != (m_FR & NegativeFlag));
		default:
			return 0;
	}
}

/////////////// Loads ///////////////

void Interpreter::RegisterLDI()
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	m_Registers[l_Addr] = l_IVal;
}

void Interpreter::StackLDI()
{
	// TODO : Check for stack fetch
	++m_PC;
	UInt16 l_IVal = FetchImmediateValue();
	m_SP = l_IVal;
}

void Interpreter::DirectLDM()
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	m_Registers[l_Addr] = (m_Memory[l_IVal+1] << 8) | m_Memory[l_IVal];
}

void Interpreter::IndirectLDM()
{
	UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
	UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
	UInt16 l_Val = (m_Memory[m_Registers[l_AddrY]+1] << 8) | m_Memory[m_Registers[l_AddrY]];
	m_Registers[l_AddrX] = l_Val;
	m_PC += 3;
}

void Interpreter::MOV()
{
	UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
	UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
	m_Registers[l_AddrX] = m_Registers[l_AddrY];
	m_PC += 3;
}

/////////////// Misc ///////////////

void Interpreter::NOP() { m_PC += 3; }

void Interpreter::CLS()
{
	m_GPU.ClearScreen();
	m_PC += 3;
}

void Interpreter::VBLNK()
{
	if(!m_GPU.VBlankFlag())
	{
		m_PC--;
	}
	else
	{
		m_GPU.TurnOffVBlankFlag();
		m_PC += 3;
	}
}

void Interpreter::BGC()
{
	m_PC++;
	m_GPU.SetBackgroundColor(m_Memory[m_PC++] & 0xF);
	m_PC++;
}

void Interpreter::SPR()
{
	m_PC++;
	m_GPU.SetSpriteDimensions(m_Memory[m_PC++], m_Memory[m_PC++]);
}

void Interpreter::ImmediateDRW()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	UInt16 l_Addr = FetchImmediateValue();
	auto x = FetchSprite(l_Addr);
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, FetchSprite(l_Addr));
	m_FR = l_RetVal ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void Interpreter::RegisterDRW()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	UInt16 l_Addr = m_Registers[m_Memory[m_PC++]];
	m_PC++;
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, FetchSprite(l_Addr));
	m_FR = l_RetVal ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void Interpreter::RND()
{
	// FIXME : Not so random
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_MaxVal = FetchImmediateValue();
	UInt16 l_RandVal = m_Dist(m_RandEngine);
	while(l_RandVal > l_MaxVal)
		l_RandVal= m_Dist(m_RandEngine);
	m_Registers[l_Addr] = l_RandVal;
}

void Interpreter::FLIP()
{
	m_PC += 2;
	m_GPU.Flip(m_Memory[m_PC++]);
}

void Interpreter::SND0()
{
	m_PC += 3;
	m_SPU.Stop();
}

void Interpreter::SND1()
{
	++m_PC;
	m_SPU.PlayTone(500, FetchImmediateValue());
}

void Interpreter::SND2()
{
	++m_PC;
	m_SPU.PlayTone(1000, FetchImmediateValue());
}

void Interpreter::SND3()
{
	++m_PC;
	m_SPU.PlayTone(1500, FetchImmediateValue());
}

void Interpreter::SNP()
{
	m_SPU.PlayTone(FetchRegisterAddress(), FetchImmediateValue());
}

void Interpreter::SNG()
{
	m_SPU.GenerateSound((m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF, 
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF,
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF);
}

/////////////// Palettes ///////////////

void Interpreter::ImmediatePalette()
{
	UInt8 l_PaletteData[16][3];	// Per specifications
	++m_PC;
	UInt16 l_Addr = FetchImmediateValue();
	for(int i = 0, j = 0; i < 16, j < 16*3; ++i, j+=3)
	{
		l_PaletteData[i][2] = m_Memory[l_Addr+j];
		l_PaletteData[i][1] = m_Memory[l_Addr+j+1];
		l_PaletteData[i][0] = m_Memory[l_Addr+j+2];
	}
	m_GPU.LoadPalette(l_PaletteData);
}

void Interpreter::RegisterPalette()
{
	UInt8 l_PaletteData[16][3];	// Per specifications
	UInt16 l_Addr = m_Registers[FetchRegisterAddress()];

	for(int i = 0, j = 0; i < 16, j < 16*3; ++i, j+=3)
	{
		l_PaletteData[i][2] = m_Memory[l_Addr+j];
		l_PaletteData[i][1] = m_Memory[l_Addr+j+1];
		l_PaletteData[i][0] = m_Memory[l_Addr+j+2];
	}
	m_PC += 3;

	m_GPU.LoadPalette(l_PaletteData);
}

/////////////// Push/Pop ///////////////
// TODO : Check for stack overflow

void Interpreter::PUSH()
{
	Push(m_Registers[m_Memory[m_PC]]);
	m_PC += 3;
}

void Interpreter::POP()
{
	m_Registers[m_Memory[m_PC]] = Pop();
	m_PC += 3;
}

void Interpreter::PUSHALL()
{
	for(UInt16 i = 0; i < 16; ++i)
		Push(m_Registers[i]);
	m_PC += 3;
}

void Interpreter::POPALL()
{
	for(Int16 i = 15; i > -1; --i)
		m_Registers[i] = Pop();
	m_PC += 3;
}

void Interpreter::PUSHF()
{
	m_Memory[m_SP] = m_FR;
	m_SP += 2;
	m_PC += 3;
}

void Interpreter::POPF()
{
	m_FR = m_Memory[m_SP];
	m_SP -= 2;
	m_PC += 3;
}

/////////////// Shift ///////////////

void Interpreter::NSHL()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void Interpreter::NSHR()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void Interpreter::NSAR()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void Interpreter::RegisterSHL()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC += 2;
}

void Interpreter::RegisterSHR()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC+=2;
}

void Interpreter::RegisterSAR()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC+=2;
}

/////////////// Store ///////////////
// TODO : Check for stack overflow

void Interpreter::DirectSTM()
{
	UInt16 l_RegAddr = FetchRegisterAddress();
	UInt16 l_MemAddr = FetchImmediateValue();
	m_Memory[l_MemAddr] = m_Registers[l_RegAddr] & 0x00FF;
	m_Memory[l_MemAddr+1] = m_Registers[l_RegAddr] >> 8;
}

void Interpreter::IndirectSTM()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_Memory[l_YVal] = l_XVal & 0x00FF;
	m_Memory[l_YVal+1] = l_XVal >> 8;
	m_PC += 3;
}

void Interpreter::BasicArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	m_Registers[m_Memory[++m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
	m_PC += 2;
}

void Interpreter::DiscardArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	UInt16 l_Result = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(l_Result);
	m_PC += 3;
}

void Interpreter::DiscardImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	in_FRH(m_Registers[l_Addr], l_IVal);
	UInt16 l_Result = in_Ins(m_Registers[l_Addr], l_IVal);
	SetSignZeroFlag(l_Result);
}

void Interpreter::ImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	in_FRH(m_Registers[l_Addr], l_IVal);
	m_Registers[l_Addr] = in_Ins(m_Registers[l_Addr], l_IVal);
	SetSignZeroFlag(m_Registers[l_Addr]);
}

void Interpreter::InplaceArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	m_Registers[m_Memory[m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
	m_PC += 3;
}
