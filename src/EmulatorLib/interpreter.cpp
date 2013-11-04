#include "interpreter.h"

#include <iostream>

Interpreter::Interpreter(const std::shared_ptr<CPU> & in_CPU) : m_CPU(in_CPU), m_Dist(0, std::numeric_limits<UInt16>::max())
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

unsigned Interpreter::InitDevices()
{
	unsigned l_ErrorCode = NoError;
	l_ErrorCode |= m_GPU.Init();
	l_ErrorCode |= m_SPU.Init();
	return l_ErrorCode;
}

unsigned Interpreter::InterpretOp()
{
	UInt32 l_Instruction = m_CPU->FetchInstruction();
	Instruction l_Func = m_Ops[l_Instruction >> 24];
	if(l_Func)
		(this->*l_Func)(l_Instruction);
	else
		std::cout << "Unknown opcode" << std::endl;
	
	return 0; // TODO : Error code
}

void Interpreter::Show()
{
	m_GPU.FlushBuffer();
}

/////////////// Arithmetic ///////////////

void Interpreter::ADDI(const UInt32 in_Instruction) 
{
	ImmediateArithmetic(in_Instruction, std::plus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2) { m_CPU->SetCarryOverflowFlagAdd(in_Op1, in_Op2); });
}

void Interpreter::InplaceADD(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::plus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2) { m_CPU->SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); 
}

void Interpreter::ADD(const UInt32 in_Instruction)
{ 
	BasicArithmetic(in_Instruction, std::plus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagAdd(in_Op1, in_Op2); });
}

void Interpreter::SUBI(const UInt32 in_Instruction) 
{
	ImmediateArithmetic(in_Instruction, std::minus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::InplaceSUB(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::minus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::SUB(const UInt32 in_Instruction) 
{
	BasicArithmetic(in_Instruction, std::minus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::CMPI(const UInt32 in_Instruction) 
{ 
	DiscardImmediateArithmetic(in_Instruction, std::minus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::CMP(const UInt32 in_Instruction) 
{ 
	DiscardArithmetic(in_Instruction, std::minus<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagSub(in_Op1, in_Op2); });
}

void Interpreter::ANDI(const UInt32 in_Instruction) 
{ 
	ImmediateArithmetic(in_Instruction, std::bit_and<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::InplaceAND(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::bit_and<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::AND(const UInt32 in_Instruction) 
{ 
	BasicArithmetic(in_Instruction, std::bit_and<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::TSTI(const UInt32 in_Instruction) 
{ 
	DiscardImmediateArithmetic(in_Instruction, std::bit_and<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::TST(const UInt32 in_Instruction) 
{ 
	DiscardArithmetic(in_Instruction, std::bit_and<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::ORI(const UInt32 in_Instruction) 
{ 
	ImmediateArithmetic(in_Instruction, std::bit_or<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::InplaceOR(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::bit_or<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::OR(const UInt32 in_Instruction) 
{ 
	BasicArithmetic(in_Instruction, std::bit_or<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::XORI(const UInt32 in_Instruction) 
{ 
	ImmediateArithmetic(in_Instruction, std::bit_xor<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::InplaceXOR(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::bit_xor<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::XOR(const UInt32 in_Instruction) 
{ 
	BasicArithmetic(in_Instruction, std::bit_xor<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlag(in_Op1, in_Op2); });
}

void Interpreter::MULI(const UInt32 in_Instruction) 
{ 
	ImmediateArithmetic(in_Instruction, std::multiplies<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::InplaceMUL(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::multiplies<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::MUL(const UInt32 in_Instruction) 
{ 
	BasicArithmetic(in_Instruction, std::multiplies<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagMul(in_Op1, in_Op2); });
}

void Interpreter::DIVI(const UInt32 in_Instruction) 
{ 
	ImmediateArithmetic(in_Instruction, std::divides<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

void Interpreter::InplaceDIV(const UInt32 in_Instruction) 
{ 
	InplaceArithmetic(in_Instruction, std::divides<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

void Interpreter::DIV(const UInt32 in_Instruction) 
{ 
	BasicArithmetic(in_Instruction, std::divides<UInt16>(),
		[this](UInt16 in_Op1, UInt16 in_Op2){ m_CPU->SetCarryOverflowFlagDiv(in_Op1, in_Op2); });
}

/////////////// Call/Jump ///////////////

void Interpreter::DirectJMP(const UInt32 in_Instruction)
{
	m_CPU->SetProgramCounter(FetchImmediateValue(in_Instruction));
}

void Interpreter::Jx(const UInt32 in_Instruction)
{
	UInt8 l_CondCode = FetchHalfByte(in_Instruction, 4);
	if(InterpretConditions(l_CondCode))
		m_CPU->SetProgramCounter(FetchImmediateValue(in_Instruction));
}

void Interpreter::JME(const UInt32 in_Instruction)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	if(l_XVal == l_YVal)
		m_CPU->SetProgramCounter(FetchImmediateValue(in_Instruction));
}

void Interpreter::DirectCALL(const UInt32 in_Instruction)
{
	m_CPU->PushPC();
	m_CPU->SetProgramCounter(FetchImmediateValue(in_Instruction));
}

void Interpreter::RET(const UInt32 in_Instruction)
{
	m_CPU->SetProgramCounter(m_CPU->Pop());
}

void Interpreter::IndirectJMP(const UInt32 in_Instruction)
{
	m_CPU->SetProgramCounter(m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4)));
}

void Interpreter::Cx(const UInt32 in_Instruction)
{
	UInt8 l_CondCode = (in_Instruction >> 16) & 0xF;
	if(InterpretConditions(l_CondCode))
	{
		m_CPU->PushPC();
		m_CPU->SetProgramCounter(FetchImmediateValue(in_Instruction));
	}
}

void Interpreter::IndirectCALL(const UInt32 in_Instruction)
{
	m_CPU->PushPC();
	m_CPU->SetProgramCounter(m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4)));
}

unsigned Interpreter::InterpretConditions(UInt8 in_CondCode)
{
	UInt16 l_FR = m_CPU->DumpFlagRegister();
	switch (in_CondCode & 0xF)
	{
		case 0x0:	// Z
			return l_FR & CPU::ZeroFlag;
		case 0x1:	// NZ
			return l_FR ^ (l_FR | CPU::ZeroFlag);
		case 0x2:	// N
			return l_FR & CPU::NegativeFlag;
		case 0x3:	// NN
			return (l_FR ^ (l_FR | CPU::NegativeFlag));
		case 0x4:	// P
			return !(l_FR & CPU::ZeroFlag) && (l_FR ^ (l_FR | CPU::NegativeFlag));
		case 0x5:	// O
			return l_FR & CPU::SignedOverflowFlag;
		case 0x6:	// NO
			return l_FR ^ (l_FR | CPU::SignedOverflowFlag);
		case 0x7:	// A
			return (l_FR ^ (l_FR | CPU::ZeroFlag)) && (l_FR ^ (l_FR | CPU::SignedOverflowFlag));
		case 0x8:	// AE
			return l_FR ^ (l_FR | CPU::UnsignedCarryFlag);
		case 0x9:	// B
			return l_FR & CPU::UnsignedCarryFlag;
		case 0xA:	// BE
			return (l_FR & CPU::ZeroFlag) || (l_FR & CPU::UnsignedCarryFlag);
		case 0xB:	// G
			return (l_FR & CPU::ZeroFlag) && ((l_FR & CPU::SignedOverflowFlag) == (l_FR & CPU::NegativeFlag));
		case 0xC:	// GE
			return ((l_FR & CPU::SignedOverflowFlag) == (l_FR & CPU::NegativeFlag));
		case 0xD:	// L
			return ((l_FR & CPU::SignedOverflowFlag) != (l_FR & CPU::NegativeFlag));
		case 0xE:	// LE
			return l_FR & CPU::ZeroFlag || ((l_FR & CPU::SignedOverflowFlag) != (l_FR & CPU::NegativeFlag));
		default:
			return 0;
	}
}

/////////////// Loads ///////////////

void Interpreter::RegisterLDI(const UInt32 in_Instruction)
{
	m_CPU->SetRegister(FetchHalfByte(in_Instruction, 4), FetchImmediateValue(in_Instruction));
}

void Interpreter::StackLDI(const UInt32 in_Instruction)
{
	m_CPU->SetStackPointer(FetchImmediateValue(in_Instruction));
}

void Interpreter::DirectLDM(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	UInt16 l_IVal = FetchImmediateValue(in_Instruction);
	m_CPU->SetRegister(l_Addr, m_CPU->Load(l_IVal));
}

void Interpreter::IndirectLDM(const UInt32 in_Instruction)
{
	UInt8 l_AddrX = FetchHalfByte(in_Instruction, 4);
	UInt8 l_AddrY = FetchHalfByte(in_Instruction, 5);
	m_CPU->SetRegister(l_AddrX, m_CPU->Load(m_CPU->DumpRegister(l_AddrY)));
}

void Interpreter::MOV(const UInt32 in_Instruction)
{
	UInt8 l_AddrX = FetchHalfByte(in_Instruction, 4);
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5)); 
	m_CPU->SetRegister(l_AddrX, l_YVal);
}

/////////////// Misc ///////////////

void Interpreter::NOP(const UInt32 in_Instruction) {  }

void Interpreter::CLS(const UInt32 in_Instruction)
{
	m_GPU.ClearScreen();
}

void Interpreter::VBLNK(const UInt32 in_Instruction)
{
	if(!m_GPU.VBlankFlag())
		m_CPU->StepBack();
	else
		m_GPU.TurnOffVBlankFlag();
}

void Interpreter::BGC(const UInt32 in_Instruction)
{
	m_GPU.SetBackgroundColor((in_Instruction >> 8) & 0xF);
}

void Interpreter::SPR(const UInt32 in_Instruction)
{
	m_GPU.SetSpriteDimensions((in_Instruction >> 8) & 0xFF, in_Instruction & 0xFF);
}

void Interpreter::ImmediateDRW(const UInt32 in_Instruction)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	UInt16 l_Addr = FetchImmediateValue(in_Instruction);
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, m_CPU->FetchSprite(l_Addr, m_GPU.SpriteWidth(), m_GPU.SpriteHeight()));
	if(l_RetVal)
		m_CPU->SetFlag(CPU::UnsignedCarryFlag);
	else m_CPU->UnsetFlag(CPU::UnsignedCarryFlag);
}

void Interpreter::RegisterDRW(const UInt32 in_Instruction)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	UInt16 l_Addr = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 2));
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, m_CPU->FetchSprite(l_Addr, m_GPU.SpriteWidth(), m_GPU.SpriteHeight()));
	if(l_RetVal)
		m_CPU->SetFlag(CPU::UnsignedCarryFlag);
	else m_CPU->UnsetFlag(CPU::UnsignedCarryFlag);
}

void Interpreter::RND(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	UInt16 l_MaxVal = FetchImmediateValue(in_Instruction);
	UInt16 l_RandVal = m_Dist(m_RandEngine);
	while(l_RandVal > l_MaxVal)
		l_RandVal= m_Dist(m_RandEngine);
	m_CPU->SetRegister(l_Addr, l_RandVal);
}

void Interpreter::FLIP(const UInt32 in_Instruction)
{
	m_GPU.Flip(in_Instruction & 0xF);
}

void Interpreter::SND0(const UInt32 in_Instruction)
{
	m_SPU.Stop();
}

void Interpreter::SND1(const UInt32 in_Instruction)
{
	m_SPU.PlayTone(UInt8(500), FetchImmediateValue(in_Instruction));
}

void Interpreter::SND2(const UInt32 in_Instruction)
{
	m_SPU.PlayTone(UInt8(1000), FetchImmediateValue(in_Instruction));
}

void Interpreter::SND3(const UInt32 in_Instruction)
{
	m_SPU.PlayTone(UInt8(1500), FetchImmediateValue(in_Instruction));
}

void Interpreter::SNP(const UInt32 in_Instruction)
{
	m_SPU.PlayTone(FetchHalfByte(in_Instruction, 4), FetchImmediateValue(in_Instruction));
}

void Interpreter::SNG(const UInt32 in_Instruction)
{
	// TODO...
	/*m_SPU->GenerateSound((m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF, 
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF,
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF);*/
}

/////////////// Palettes ///////////////

void Interpreter::ImmediatePalette(const UInt32 in_Instruction)
{
	m_GPU.LoadPalette(m_CPU->FetchPalette(FetchImmediateValue(in_Instruction)));
}

void Interpreter::RegisterPalette(const UInt32 in_Instruction)
{
	m_GPU.LoadPalette(m_CPU->FetchPalette(m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4))));
}

/////////////// Push/Pop ///////////////

void Interpreter::PUSH(const UInt32 in_Instruction)
{
	m_CPU->Push(m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4)));
}

void Interpreter::POP(const UInt32 in_Instruction)
{
	m_CPU->SetRegister(FetchHalfByte(in_Instruction, 4), m_CPU->Pop());
}

void Interpreter::PUSHALL(const UInt32 in_Instruction)
{
	for(UInt8 i = 0; i < 16; ++i)
		m_CPU->Push(m_CPU->DumpRegister(i));
}

void Interpreter::POPALL(const UInt32 in_Instruction)
{
	for(int i = 15; i > -1; --i)
		m_CPU->SetRegister(i, m_CPU->Pop());
}

void Interpreter::PUSHF(const UInt32 in_Instruction)
{
	m_CPU->Push(m_CPU->DumpFlagRegister());
}

void Interpreter::POPF(const UInt32 in_Instruction)
{
	m_CPU->SetFlagRegister(m_CPU->Pop());
}

/////////////// Shift ///////////////

void Interpreter::NSHL(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, LeftShift()(m_CPU->DumpRegister(l_Addr), FetchHalfByte(in_Instruction, 2)));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

void Interpreter::NSHR(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, LogicalRightShift()(m_CPU->DumpRegister(l_Addr), FetchHalfByte(in_Instruction, 2)));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

void Interpreter::NSAR(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, ArithmeticRightShift()(m_CPU->DumpRegister(l_Addr), FetchHalfByte(in_Instruction, 2)));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

void Interpreter::RegisterSHL(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, LeftShift()(m_CPU->DumpRegister(l_Addr), m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5))));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

void Interpreter::RegisterSHR(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, LogicalRightShift()(m_CPU->DumpRegister(l_Addr), m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5))));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

void Interpreter::RegisterSAR(const UInt32 in_Instruction)
{
	UInt8 l_Addr = FetchHalfByte(in_Instruction, 4);
	m_CPU->SetRegister(l_Addr, ArithmeticRightShift()(m_CPU->DumpRegister(l_Addr), m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5))));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Addr));
}

/////////////// Store ///////////////

void Interpreter::DirectSTM(const UInt32 in_Instruction)
{
	UInt8 l_RegAddr = FetchHalfByte(in_Instruction, 4);
	UInt16 l_MemAddr = FetchImmediateValue(in_Instruction);
	m_CPU->Store(l_MemAddr, m_CPU->DumpRegister(l_RegAddr));
}

void Interpreter::IndirectSTM(const UInt32 in_Instruction)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	m_CPU->Store(l_YVal, l_XVal);
}

/////////////// Artihmetic Helpers ///////////////

void Interpreter::BasicArithmetic(const UInt32 in_Instruction, 
								  std::function<UInt16(UInt16,UInt16)> in_Ins, 
								  std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	in_FRH(l_XVal, l_YVal);
	UInt8 l_ZReg = FetchHalfByte(in_Instruction, 2);
	m_CPU->SetRegister(l_ZReg, in_Ins(l_XVal, l_YVal));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_ZReg));
}

void Interpreter::DiscardArithmetic(const UInt32 in_Instruction, 
									std::function<UInt16(UInt16,UInt16)> in_Ins, 
									std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	in_FRH(l_XVal, l_YVal);
	UInt16 l_Result = in_Ins(l_XVal, l_YVal);
	m_CPU->SetSignZeroFlag(l_Result);
}

void Interpreter::DiscardImmediateArithmetic(const UInt32 in_Instruction, 
											 std::function<UInt16(UInt16,UInt16)> in_Ins, 
											 std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 4));
	UInt16 l_IVal = FetchImmediateValue(in_Instruction);
	in_FRH(l_XVal, l_IVal);
	UInt16 l_Result = in_Ins(l_XVal, l_IVal);
	m_CPU->SetSignZeroFlag(l_Result);
}

void Interpreter::ImmediateArithmetic(const UInt32 in_Instruction, 
									  std::function<UInt16(UInt16,UInt16)> in_Ins, 
									  std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt8 l_Reg = FetchHalfByte(in_Instruction, 4);
	UInt16 l_IVal = FetchImmediateValue(in_Instruction);
	in_FRH(m_CPU->DumpRegister(l_Reg), l_IVal);
	m_CPU->SetRegister(l_Reg, in_Ins(m_CPU->DumpRegister(l_Reg), l_IVal));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_Reg));
}

void Interpreter::InplaceArithmetic(const UInt32 in_Instruction, 
									std::function<UInt16(UInt16,UInt16)> in_Ins, 
									std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt8 l_XReg = FetchHalfByte(in_Instruction, 4);
	UInt16 l_XVal = m_CPU->DumpRegister(l_XReg);
	UInt16 l_YVal = m_CPU->DumpRegister(FetchHalfByte(in_Instruction, 5));
	in_FRH(l_XVal, l_YVal);
	m_CPU->SetRegister(l_XReg, in_Ins(l_XVal, l_YVal));
	m_CPU->SetSignZeroFlag(m_CPU->DumpRegister(l_XReg));
}

/////////////// Helpers ///////////////

UInt16 Interpreter::FetchImmediateValue(const UInt32 in_Instruction) const
{
	UInt16 l_IVal = (in_Instruction >> 8) & 0xFF;
	return l_IVal | ((in_Instruction & 0xFF) << 8);
}

UInt8 Interpreter::FetchHalfByte(const UInt32 in_Instruction, const UInt8 in_Pos) const
{
	return (in_Instruction >> (in_Pos*4)) & 0xF;
}
