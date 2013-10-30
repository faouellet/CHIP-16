#include "cpu.h"

#include <functional>
#include <iterator>

CPU::CPU() : m_FR(0), m_PC(0), m_SP(0), m_ErrorCode(0)
{
	memset(m_Registers, 0, sizeof(UInt16)*16);
	memset(m_Memory, 0, sizeof(UInt8)*MEMORY_SIZE);
}

CPU::~CPU() { }

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

UInt16 CPU::DumpRegister(const UInt8 in_RegID) const
{
	// TODO : Check for correct register ID
	return m_Registers[in_RegID];
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

	return NoError;
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

void CPU::SetProgramCounter(const UInt16 in_Value)
{
	// TODO : Check for correct PC value
	m_PC = in_Value;
}

void CPU::SetRegister(const UInt8 in_RegID, const UInt16 in_Value)
{
	// TODO : Check for correct register ID
	m_Registers[in_RegID] = in_Value;
}

void CPU::SetStackPointer(const UInt16 in_Value)
{
	// TODO : Check for correct SP value
	m_SP = in_Value;
}

void CPU::UpdateController(SDL_KeyboardEvent & in_Event)
{
	std::function<void(const UInt16 in_ControllerID, const UInt16 in_Mask)> l_UpdateControllerData;
	if(in_Event.type == SDL_KEYDOWN)
		l_UpdateControllerData = [this](const UInt16 in_ControllerID, const UInt16 in_Mask){ m_Memory[in_ControllerID] |= in_Mask; };
	else
		l_UpdateControllerData = [this](const UInt16 in_ControllerID, const UInt16 in_Mask){ m_Memory[in_ControllerID] &= ~in_Mask; };

	switch (in_Event.keysym.sym)
	{
		case SDLK_UP:
		{
			l_UpdateControllerData(CONTROLLER_1, UP);
			break;
		}
		case SDLK_DOWN:
		{
			l_UpdateControllerData(CONTROLLER_1, DOWN);
			break;
		}
		case SDLK_LEFT:
		{
			l_UpdateControllerData(CONTROLLER_1, LEFT);
			break;
		}
		case SDLK_RIGHT:
		{
			l_UpdateControllerData(CONTROLLER_1, RIGHT);
			break;
		}
		case SDLK_RSHIFT:
		{
			l_UpdateControllerData(CONTROLLER_1, SELECT);
			break;
		}
		case SDLK_RETURN:
		{
			l_UpdateControllerData(CONTROLLER_1, START);
			break;
		}
		case SDLK_o:
		{
			l_UpdateControllerData(CONTROLLER_1, A);
			break;
		}
		case SDLK_p:
		{
			l_UpdateControllerData(CONTROLLER_1, B);
			break;
		}
		case SDLK_w:
		{
			l_UpdateControllerData(CONTROLLER_2, UP);
			break;
		}
		case SDLK_s:
		{
			l_UpdateControllerData(CONTROLLER_2, DOWN);
			break;
		}
		case SDLK_a:
		{
			l_UpdateControllerData(CONTROLLER_2, LEFT);
			break;
		}
		case SDLK_d:
		{
			l_UpdateControllerData(CONTROLLER_2, RIGHT);
			break;
		}
		case SDLK_LSHIFT:
		{
			l_UpdateControllerData(CONTROLLER_2, SELECT);
			break;
		}
		case SDLK_SPACE:
		{
			l_UpdateControllerData(CONTROLLER_2, START);
			break;
		}
		case SDLK_v:
		{
			l_UpdateControllerData(CONTROLLER_2, A);
			break;
		}
		case SDLK_b:
		{
			l_UpdateControllerData(CONTROLLER_2, B);
			break;
		}
	}
}

UInt32 CPU::FetchInstruction()
{
	UInt32 l_Instruction = 0;

	for(int i = 0; i < 4; ++i)
		l_Instruction = (l_Instruction << (i*4)) | m_Memory[m_PC++];

	return l_Instruction;
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

void CPU::PushPC()
{
	m_Memory[m_SP++] = m_PC & 0x00FF;
	m_Memory[m_SP++] = (m_PC & 0xFF00) >> 8;
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
