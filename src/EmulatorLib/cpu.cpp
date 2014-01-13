#include "cpu.h"

#include <functional>
#include <iostream>
#include <iterator>
#include <limits>

#ifdef __unix__ 
#include <string.h>
#endif

CPU::CPU() : m_FR(0), m_PC(0), m_SP(0), m_ErrorCode(0)
{
	memset(m_Registers, 0, sizeof(UInt16)*16);
	memset(m_Memory, 0, sizeof(UInt8)*MEMORY_SIZE);
}

CPU::CPU(const CPU & in_CPU) : m_FR(in_CPU.m_FR), m_PC(in_CPU.m_PC), 
	m_SP(in_CPU.m_SP), m_ErrorCode(in_CPU.m_ErrorCode)
{
	int i = 0;
	for(auto in_Val : in_CPU.DumpRegisters())
		m_Registers[i++] = in_Val;

	i = 0;
	for(auto in_Val : in_CPU.m_ROMHeader)
		m_ROMHeader[i++] = in_Val;

	i = 0;
	for(auto in_Val : in_CPU.DumpMemory())
		m_Memory[i++] = in_Val;
}

CPU::CPU(const CPU * in_CPU) : m_FR(in_CPU->m_FR), m_PC(in_CPU->m_PC), 
	m_SP(in_CPU->m_SP), m_ErrorCode(in_CPU->m_ErrorCode)
{
	int i = 0;
	for(auto in_Val : in_CPU->DumpRegisters())
		m_Registers[i++] = in_Val;

	i = 0;
	for(auto in_Val : in_CPU->m_ROMHeader)
		m_ROMHeader[i++] = in_Val;

	i = 0;
	for(auto in_Val : in_CPU->DumpMemory())
		m_Memory[i++] = in_Val;
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

void CPU::SetFlagRegister(const UInt16 in_Value)
{
	// Discarding unused bits
	m_FR = in_Value & 0xFF;
}

void CPU::SetFlag(const UInt16 in_Value)
{
	m_FR |= in_Value;
}

void CPU::UnsetFlag(const UInt16 in_Value)
{
	m_FR &= ~in_Value;
}

UInt8 CPU::SetProgramCounter(const UInt16 in_Value)
{
	if(in_Value > STACK_START)
	{
		std::cout << "Not a valid value for the PC: " << std::hex << in_Value << std::endl;
		return MemoryError;
	}
	else
	{
		m_PC = in_Value;
		return NoError;
	}
}

UInt8 CPU::SetRegister(const UInt8 in_RegID, const UInt16 in_Value)
{
	if(in_RegID > NB_REGISTERS)
	{
		std::cout << "Not a valid register ID: " << std::hex << in_RegID << std::endl;
		return UnknownRegister;
	}
	else
	{
		m_Registers[in_RegID] = in_Value;
		return NoError;
	}
}

UInt8 CPU::SetStackPointer(const UInt16 in_Value)
{
	
	if(in_Value < STACK_START || in_Value > CONTROLLER_1)
	{
		std::cout << "Not a valid value for the SP: " << std::hex << in_Value << std::endl;
		return MemoryError;
	}
	else
	{
		m_SP = in_Value;
		return NoError;
	}
}

void CPU::StepBack()
{
	m_PC -= 4;
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

std::vector<Instruction> CPU::FetchBasicBlock()
{
	Instruction l_Instruction = 0;
	std::vector<Instruction> l_BasicBlock;

	do
	{
		// Q : Should the PC be modified ??
		l_Instruction = FetchInstruction();

		l_BasicBlock.push_back(l_Instruction);
	} 
	while((l_Instruction.GetOpcode() & 0xF0) != 0x10);

	return l_BasicBlock;
}

Instruction CPU::FetchInstruction()
{
	UInt32 l_Instruction = 0;

	for(int i = 0; i < 4; ++i)
	{
		l_Instruction <<= 8;
		l_Instruction |= m_Memory[m_PC++];
	}

	return Instruction(l_Instruction);
}

std::vector<UInt8> CPU::FetchPalette(const UInt16 in_Address)
{
	std::vector<UInt8> l_PaletteData(16*3);	// Per specifications

	for(int j = 0; j < 16*3; j+=3)
	{
		l_PaletteData[j+2] = m_Memory[in_Address+j];
		l_PaletteData[j+1] = m_Memory[in_Address+j+1];
		l_PaletteData[j] = m_Memory[in_Address+j+2];
	}

	return l_PaletteData;
}

void CPU::FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y) const
{
	out_X = m_Registers[m_Memory[m_PC] & 0xF];
	out_Y = m_Registers[(m_Memory[m_PC] & 0xF0) >> 4];
}

std::vector<UInt8> CPU::FetchSprite(const UInt16 in_Addr, const UInt16 in_Width, const UInt16 in_Height) const
{
	std::vector<UInt8> l_SpriteData(in_Width * in_Height);

	for(int i = 0; i < (in_Width * in_Height)/2; ++i)
	{
		l_SpriteData[2*i] = m_Memory[in_Addr + i] >> 4;
		l_SpriteData[2*i+1] = m_Memory[in_Addr + i] & 0xF;
	}

	return l_SpriteData;
}

UInt8 CPU::Load(const UInt16 in_Address, UInt16 & out_Value) const
{
	if(STACK_START < in_Address && in_Address < CONTROLLER_1)
	{
		std::cout << "Address out of memory: " << std::hex << in_Address << std::endl;
		return MemoryError;
	}
	else
	{
		out_Value = (m_Memory[in_Address+1] << 8) | m_Memory[in_Address];
		return NoError;
	}
}

UInt8 CPU::Store(const UInt16 in_Address, const UInt16 in_Value)
{
	if(in_Address > STACK_START)
	{
		std::cout << "Address out of memory: " << std::hex << in_Address << std::endl;
		return MemoryError;
	}
	else
	{
		m_Memory[in_Address] = in_Value & 0x00FF;
		m_Memory[in_Address+1] = in_Value >> 8;
		return NoError;
	}
}

UInt8 CPU::Pop(UInt16 & out_Value)
{
	if(m_SP-2 < STACK_START)
	{
		std::cout << "Stack underflow" << std::endl;
		return StackUnderflow;
	}
	else
	{
		out_Value = UInt16(m_Memory[--m_SP] << 8 | (m_Memory[--m_SP]));
		return NoError;
	}
}

UInt8 CPU::Push(UInt16 in_Val)
{
	if(m_SP+2 > CONTROLLER_1)
	{
		std::cout << "Stack overflow while pushing: " << in_Val << std::endl;
		return StackOverflow;
	}
	else
	{
		m_Memory[m_SP++] = in_Val & 0x00FF;
		m_Memory[m_SP++] = (in_Val & 0xFF00) >> 8;
		return NoError;
	}
}

UInt8 CPU::PushPC()
{
	return Push(m_PC);
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
