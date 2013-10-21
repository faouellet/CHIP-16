#include "cpu.h"

#include <iostream>
#include <iterator>
#include <limits>

CPU::CPU() : m_FR(0), m_PC(0), m_SP(0), m_ErrorCode(0), m_Dist(0, std::numeric_limits<UInt16>::max())
{
	for(int i = 0; i < 16; ++i)
		m_Registers[i] = 0;

	for(int i = 0; i < MEMORY_SIZE; ++i)
		m_Memory[i] = 0;

	m_Ops[0x00] = &CPU::NOP; 
	m_Ops[0x01] = &CPU::CLS;
	m_Ops[0x02] = &CPU::VBLNK; 
	m_Ops[0x03] = &CPU::BGC; 
	m_Ops[0x04] = &CPU::SPR; 
	m_Ops[0x05] = &CPU::ImmediateDRW; 
	m_Ops[0x06] = &CPU::RegisterDRW; 
	m_Ops[0x07] = &CPU::RND;
	
	m_Ops[0x08] = &CPU::FLIP; 
	
	m_Ops[0x09] = &CPU::SND0; 
	m_Ops[0x0A] = &CPU::SND1;
	m_Ops[0x0B] = &CPU::SND2;
	m_Ops[0x0C] = &CPU::SND3;
	m_Ops[0x0D] = &CPU::SNP; 
	m_Ops[0x0E] = &CPU::SNG;

	m_Ops[0x10] = &CPU::DirectJMP; 
	m_Ops[0x12] = &CPU::Jx;
	m_Ops[0x13] = &CPU::JME; 
	m_Ops[0x14] = &CPU::DirectCALL; 
	m_Ops[0x15] = &CPU::RET; 
	m_Ops[0x16] = &CPU::IndirectJMP; 
	m_Ops[0x17] = &CPU::Cx; 
	m_Ops[0x18] = &CPU::IndirectCALL;

	m_Ops[0x20] = &CPU::RegisterLDI;
	m_Ops[0x21] = &CPU::StackLDI;
	m_Ops[0x22] = &CPU::DirectLDM;
	m_Ops[0x23] = &CPU::IndirectLDM; 
	m_Ops[0x24] = &CPU::MOV;

	m_Ops[0x30] = &CPU::DirectSTM; 
	m_Ops[0x31] = &CPU::IndirectSTM;

	m_Ops[0x40] = &CPU::ADDI;
	m_Ops[0x41] = &CPU::InplaceADD;
	m_Ops[0x42] = &CPU::ADD;

	m_Ops[0x50] = &CPU::SUBI;
	m_Ops[0x51] = &CPU::InplaceSUB;
	m_Ops[0x52] = &CPU::SUB;

	m_Ops[0x53] = &CPU::CMPI;
	m_Ops[0x54] = &CPU::CMP;

	m_Ops[0x60] = &CPU::ANDI;
	m_Ops[0x61] = &CPU::InplaceAND;
	m_Ops[0x62] = &CPU::AND;
	
	m_Ops[0x63] = &CPU::TSTI;
	m_Ops[0x64] = &CPU::TST;

	m_Ops[0x70] = &CPU::ORI;
	m_Ops[0x71] = &CPU::InplaceOR;
	m_Ops[0x72] = &CPU::OR;

	m_Ops[0x80] = &CPU::XORI;
	m_Ops[0x81] = &CPU::InplaceXOR;
	m_Ops[0x82] = &CPU::XOR;

	m_Ops[0x90] = &CPU::MULI;
	m_Ops[0x91] = &CPU::InplaceMUL;
	m_Ops[0x92] = &CPU::MUL;
	
	m_Ops[0xA0] = &CPU::DIVI;
	m_Ops[0xA1] = &CPU::InplaceDIV;
	m_Ops[0xA2] = &CPU::DIV;
	
	m_Ops[0xB0] = &CPU::NSHL;
	m_Ops[0xB1] = &CPU::NSHR;
	m_Ops[0xB2] = &CPU::NSAR;
	m_Ops[0xB3] = &CPU::RegisterSHL;
	m_Ops[0xB4] = &CPU::RegisterSHR;
	m_Ops[0xB5] = &CPU::RegisterSAR;

	m_Ops[0xC0] = &CPU::PUSH;
	m_Ops[0xC1] = &CPU::POP;
	m_Ops[0xC2] = &CPU::PUSHALL;
	m_Ops[0xC3] = &CPU::POPALL;
	m_Ops[0xC4] = &CPU::PUSHF;
	m_Ops[0xC5] = &CPU::POPF;

	m_Ops[0xD0] = &CPU::ImmediatePalette;
	m_Ops[0xD1] = &CPU::RegisterPalette;
}

CPU::~CPU() { }

void CPU::FlushGPU()
{
	m_GPU.FlushBuffer();
}

UInt16 CPU::DumpFlagRegister() const
{
	return m_FR;
}

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
	
	l_Ret |= m_GPU.Init();
		
	l_Ret |= m_SPU.Init();

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

	m_GPU.Reset();
	m_SPU.Reset();
}

void CPU::UpdateController(SDL_KeyboardEvent & in_Event)
{
	// TODO : Could probably use some refactoring
	
	switch (in_Event.keysym.sym)
	{
		case SDLK_UP:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | UP : m_Memory[CONTROLLER_1] & ~UP;
			break;
		}
		case SDLK_DOWN:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | DOWN : m_Memory[CONTROLLER_1] & ~DOWN;
			break;
		}
		case SDLK_LEFT:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | LEFT : m_Memory[CONTROLLER_1] & ~LEFT;
			break;
		}
		case SDLK_RIGHT:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | RIGHT : m_Memory[CONTROLLER_1] & ~RIGHT;
			break;
		}
		case SDLK_RSHIFT:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | SELECT : m_Memory[CONTROLLER_1] & ~SELECT;
			break;
		}
		case SDLK_RETURN:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | START : m_Memory[CONTROLLER_1] & ~START;
			break;
		}
		case SDLK_o:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | A : m_Memory[CONTROLLER_1] & ~A;
			break;
		}
		case SDLK_p:
		{
			m_Memory[CONTROLLER_1] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_1] | B : m_Memory[CONTROLLER_1] & ~B;
			break;
		}
		case SDLK_w:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | UP : m_Memory[CONTROLLER_2] & ~UP;
			break;
		}
		case SDLK_s:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | DOWN : m_Memory[CONTROLLER_2] & ~DOWN;
			break;
		}
		case SDLK_a:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | LEFT : m_Memory[CONTROLLER_2] & ~LEFT;
			break;
		}
		case SDLK_d:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | RIGHT : m_Memory[CONTROLLER_2] & ~RIGHT;
			break;
		}
		case SDLK_LSHIFT:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | SELECT : m_Memory[CONTROLLER_2] & ~SELECT;
			break;
		}
		case SDLK_SPACE:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | START : m_Memory[CONTROLLER_2] & ~START;
			break;
		}
		case SDLK_v:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | A : m_Memory[CONTROLLER_2] & ~A;
			break;
		}
		case SDLK_b:
		{
			m_Memory[CONTROLLER_2] = in_Event.type == SDL_KEYDOWN ? m_Memory[CONTROLLER_2] | B : m_Memory[CONTROLLER_2] & ~B;
			break;
		}
	}
}

unsigned CPU::InterpretOp()
{
	Instruction l_Func = m_Ops[m_Memory[m_PC++]];
	if(l_Func)
		(this->*l_Func)();
	else
		std::cout << "Unknown opcode" << std::endl;
	
	if(m_PC > m_ROMHeader[6])
		m_ErrorCode |= EmulationDone;
	
	return m_ErrorCode;
}

/////////////// Arithmetic ///////////////

void CPU::ADDI() { ImmediateArithmetic(std::plus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void CPU::InplaceADD() { InplaceArithmetic(std::plus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void CPU::ADD() { BasicArithmetic(std::plus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagAdd(in_Op1, in_Op2); }); }

void CPU::SUBI() { ImmediateArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void CPU::InplaceSUB() { InplaceArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void CPU::SUB() { BasicArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void CPU::CMPI() { DiscardImmediateArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void CPU::CMP() { DiscardArithmetic(std::minus<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagSub(in_Op1, in_Op2); }); }

void CPU::ANDI() { ImmediateArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::InplaceAND() { InplaceArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::AND() { BasicArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::TSTI() { DiscardImmediateArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::TST() { DiscardArithmetic(std::bit_and<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::ORI() { ImmediateArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::InplaceOR() { InplaceArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::OR() { BasicArithmetic(std::bit_or<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::XORI() { ImmediateArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::InplaceXOR() { InplaceArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::XOR() { BasicArithmetic(std::bit_xor<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlag(in_Op1, in_Op2); }); }

void CPU::MULI() { ImmediateArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void CPU::InplaceMUL() { InplaceArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void CPU::MUL() { BasicArithmetic(std::multiplies<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagMul(in_Op1, in_Op2); }); }

void CPU::DIVI() { ImmediateArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

void CPU::InplaceDIV() { InplaceArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

void CPU::DIV() { BasicArithmetic(std::divides<UInt16>(),
									   [this](UInt16 in_Op1, UInt16 in_Op2){ this->SetCarryOverflowFlagDiv(in_Op1, in_Op2); }); }

/////////////// Call/Jump ///////////////

void CPU::DirectJMP()
{
	++m_PC;
	m_PC = FetchImmediateValue();
}

void CPU::Jx()
{
	UInt8 l_CondCode = m_Memory[m_PC++];
	if(InterpretConditions(l_CondCode))
		m_PC = FetchImmediateValue();
	else
		m_PC += 2;
}

void CPU::JME()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	if(l_XVal == l_YVal)
		m_PC = FetchImmediateValue();
	else
		m_PC += 2;
}

void CPU::DirectCALL()
{
	++m_PC;
	UInt16 l_Addr = FetchImmediateValue();
	Push(m_PC);
	m_PC = l_Addr;
}

void CPU::RET()
{
	m_PC = Pop();
}

void CPU::IndirectJMP()
{
	m_PC = m_Registers[FetchRegisterAddress()];
}

void CPU::Cx()
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

void CPU::IndirectCALL()
{
	UInt16 l_Addr = FetchRegisterAddress();
    m_PC += 2;
    Push(m_PC);
    m_PC = m_Registers[l_Addr];
}

unsigned CPU::InterpretConditions(UInt8 in_CondCode)
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

void CPU::RegisterLDI()
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	m_Registers[l_Addr] = l_IVal;
}

void CPU::StackLDI()
{
	// TODO : Check for stack fetch
	++m_PC;
	UInt16 l_IVal = FetchImmediateValue();
	m_SP = l_IVal;
}

void CPU::DirectLDM()
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	m_Registers[l_Addr] = (m_Memory[l_IVal+1] << 8) | m_Memory[l_IVal];
}

void CPU::IndirectLDM()
{
	UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
	UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
	UInt16 l_Val = (m_Memory[m_Registers[l_AddrY]+1] << 8) | m_Memory[m_Registers[l_AddrY]];
	m_Registers[l_AddrX] = l_Val;
	m_PC += 3;
}

void CPU::MOV()
{
	UInt16 l_AddrX = m_Memory[m_PC] & 0xF;
	UInt16 l_AddrY = (m_Memory[m_PC] & 0xF0) >> 4;
	m_Registers[l_AddrX] = m_Registers[l_AddrY];
	m_PC += 3;
}

/////////////// Misc ///////////////

void CPU::NOP() { m_PC += 3; }

void CPU::CLS()
{
	m_GPU.ClearScreen();
	m_PC += 3;
}

void CPU::VBLNK()
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

void CPU::BGC()
{
	m_PC++;
	m_GPU.SetBackgroundColor(m_Memory[m_PC++] & 0xF);
	m_PC++;
}

void CPU::SPR()
{
	m_PC++;
	m_GPU.SetSpriteDimensions(m_Memory[m_PC++], m_Memory[m_PC++]);
}

void CPU::ImmediateDRW()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	UInt16 l_Addr = FetchImmediateValue();
	auto x = FetchSprite(l_Addr);
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, FetchSprite(l_Addr));
	m_FR = l_RetVal ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::RegisterDRW()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_PC++;
	UInt16 l_Addr = m_Registers[m_Memory[m_PC++]];
	m_PC++;
	bool l_RetVal = m_GPU.Draw(l_XVal, l_YVal, FetchSprite(l_Addr));
	m_FR = l_RetVal ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
}

void CPU::RND()
{
	// FIXME : Not so random
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_MaxVal = FetchImmediateValue();
	UInt16 l_RandVal = m_Dist(m_RandEngine);
	while(l_RandVal > l_MaxVal)
		l_RandVal= m_Dist(m_RandEngine);
	m_Registers[l_Addr] = l_RandVal;
}

void CPU::FLIP()
{
	m_PC += 2;
	m_GPU.Flip(m_Memory[m_PC++]);
}

void CPU::SND0()
{
	m_PC += 3;
	m_SPU.Stop();
}

void CPU::SND1()
{
	++m_PC;
	m_SPU.PlayTone(500, FetchImmediateValue());
}

void CPU::SND2()
{
	++m_PC;
	m_SPU.PlayTone(1000, FetchImmediateValue());
}

void CPU::SND3()
{
	++m_PC;
	m_SPU.PlayTone(1500, FetchImmediateValue());
}

void CPU::SNP()
{
	m_SPU.PlayTone(FetchRegisterAddress(), FetchImmediateValue());
}

void CPU::SNG()
{
	m_SPU.GenerateSound((m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF, 
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF,
		(m_Memory[m_PC] & 0xF0) >> 4, m_Memory[m_PC++] & 0xF);
}

/////////////// Palettes ///////////////

void CPU::ImmediatePalette()
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

void CPU::RegisterPalette()
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

void CPU::PUSH()
{
	Push(m_Registers[m_Memory[m_PC]]);
	m_PC += 3;
}

void CPU::POP()
{
	m_Registers[m_Memory[m_PC]] = Pop();
	m_PC += 3;
}

void CPU::PUSHALL()
{
	for(UInt16 i = 0; i < 16; ++i)
		Push(m_Registers[i]);
	m_PC += 3;
}

void CPU::POPALL()
{
	for(Int16 i = 15; i > -1; --i)
		m_Registers[i] = Pop();
	m_PC += 3;
}

void CPU::PUSHF()
{
	m_Memory[m_SP] = m_FR;
	m_SP += 2;
	m_PC += 3;
}

void CPU::POPF()
{
	m_FR = m_Memory[m_SP];
	m_SP -= 2;
	m_PC += 3;
}

/////////////// Shift ///////////////

void CPU::NSHL()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void CPU::NSHR()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void CPU::NSAR()
{
	UInt16 l_Addr = FetchRegisterAddress();
	m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Memory[m_PC++] & 0xF);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC++;
}

void CPU::RegisterSHL()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = LeftShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC += 2;
}

void CPU::RegisterSHR()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = LogicalRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC+=2;
}

void CPU::RegisterSAR()
{
	UInt16 l_Addr = m_Memory[m_PC] & 0xF;
	m_Registers[l_Addr] = ArithmeticRightShift()(m_Registers[l_Addr], m_Registers[(m_Memory[m_PC++] & 0xF0) >> 4]);
	SetSignZeroFlag(m_Registers[l_Addr]);
	m_PC+=2;
}

/////////////// Store ///////////////
// TODO : Check for stack overflow

void CPU::DirectSTM()
{
	UInt16 l_RegAddr = FetchRegisterAddress();
	UInt16 l_MemAddr = FetchImmediateValue();
	m_Memory[l_MemAddr] = m_Registers[l_RegAddr] & 0x00FF;
	m_Memory[l_MemAddr+1] = m_Registers[l_RegAddr] >> 8;
}

void CPU::IndirectSTM()
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	m_Memory[l_YVal] = l_XVal & 0x00FF;
	m_Memory[l_YVal+1] = l_XVal >> 8;
	m_PC += 3;
}

void CPU::BasicArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	m_Registers[m_Memory[++m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
	m_PC += 2;
}

void CPU::DiscardArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	UInt16 l_Result = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(l_Result);
	m_PC += 3;
}

void CPU::DiscardImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	in_FRH(m_Registers[l_Addr], l_IVal);
	UInt16 l_Result = in_Ins(m_Registers[l_Addr], l_IVal);
	SetSignZeroFlag(l_Result);
}

void CPU::ImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_Addr = FetchRegisterAddress();
	UInt16 l_IVal = FetchImmediateValue();
	in_FRH(m_Registers[l_Addr], l_IVal);
	m_Registers[l_Addr] = in_Ins(m_Registers[l_Addr], l_IVal);
	SetSignZeroFlag(m_Registers[l_Addr]);
}

void CPU::InplaceArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH)
{
	UInt16 l_XVal, l_YVal;
	FetchRegistersValues(l_XVal, l_YVal);
	in_FRH(l_XVal, l_YVal);
	m_Registers[m_Memory[m_PC] & 0xF] = in_Ins(l_XVal, l_YVal);
	SetSignZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
	m_PC += 3;
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

void CPU::FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y) const
{
	out_X = m_Registers[m_Memory[m_PC] & 0xF];
	out_Y = m_Registers[(m_Memory[m_PC] & 0xF0) >> 4];
}

std::vector<UInt8> CPU::FetchSprite(UInt16 in_Addr) const
{
	std::vector<UInt8> l_SpriteData(m_GPU.SpriteWidth() * m_GPU.SpriteHeight());

	// TODO : What if the address is invalid or if part of the sprite is in the stack?
	for(int i = 0; i < (m_GPU.SpriteWidth() * m_GPU.SpriteHeight())/2; ++i)
	{
		l_SpriteData[2*i] = m_Memory[in_Addr + i] >> 4;
		l_SpriteData[2*i+1] = m_Memory[in_Addr + i] & 0xF;
	}

	return l_SpriteData;
}

UInt16 CPU::Pop()
{
	return UInt16(m_Memory[--m_SP] << 8 | (m_Memory[--m_SP]));
}

void CPU::Push(UInt16 in_Val)
{
	m_Memory[m_SP++] = in_Val & 0x00FF;
	m_Memory[m_SP++] = (in_Val & 0xFF00) >> 8;
}

void CPU::SetSignZeroFlag(UInt16 in_Result)
{
	// Set the zero flag (Bit[2])
	m_FR = in_Result == 0 ? m_FR | ZeroFlag : m_FR & ~ZeroFlag;
	// Set the negative flag (Bit[7])
	m_FR = in_Result & 0x8000 ? m_FR | NegativeFlag : m_FR & ~NegativeFlag;
}

void CPU::SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2) { }

void CPU::SetCarryOverflowFlagAdd(UInt16 in_Op1, UInt16 in_Op2) 
{
	UInt16 l_Result = in_Op1 + in_Op2;
	// Set carry flag
	m_FR = l_Result < in_Op1 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 & 0x8000 && in_Op2 & 0x8000)
		|| (l_Result & 0x8000 && in_Op1 >= 0 && in_Op2 >= 0) ?
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
	UInt32 l_Result = in_Op1 - in_Op2;
	// Set carry flag
	m_FR = l_Result & 0x10000 ? m_FR | UnsignedCarryFlag : m_FR & ~UnsignedCarryFlag;
	// Set overflow flag
	m_FR = (l_Result >= 0 && in_Op1 & 0x8000 && in_Op2 >= 0)
		|| (l_Result & 0x8000 && in_Op1 >= 0 && in_Op2 & 0x8000) ?
		m_FR | SignedOverflowFlag : m_FR & ~SignedOverflowFlag;
}
