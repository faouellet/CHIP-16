#include "emitter.h"

Emitter::Emitter() : m_Arena(MemoryArena())
{
	m_BufferPtr = reinterpret_cast<Utils::UInt8*>(m_Arena.GetPtr());
}

Emitter::~Emitter()
{
	m_BufferPtr = nullptr;
}
	
UInt8 Emitter::ModRM(const UInt8 in_Mod, const UInt8 in_Reg1, const UInt8 in_Reg2) const
{
	return UInt8((in_Mod << 6) | (in_Reg1 << 3) | in_Reg2);
}

void Emitter::Write8Bits(const UInt8 in_Value)
{
	*m_BufferPtr++ = in_Value;
}

void Emitter::Write16Bits(const UInt16 in_Value)
{
	*m_BufferPtr++ = in_Value & 0xFF;
	*m_BufferPtr++ = in_Value & 0xFF00;
}

/////////////// Arithmetic ///////////////
// TODO : Refactor immediate arithmetic instructions generation
// TODO : Review the parameters for *REG generators (should only be register IDs?)
// Q : Should the args be written in the instructions in in *REG generators ??

void Emitter::ANDIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 4, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::ANDREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x23);
	Write8Bits(ModRM(3, in_Arg1, in_Arg1));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::ADDIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 0, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::ADDREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x03);
	Write8Bits(ModRM(3, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::CMPIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 7, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::CMPREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x39);
	Write8Bits(ModRM(3, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::DIVIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::DIVREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::MULIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::MULREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::ORIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 1, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::ORREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x09);
	Write8Bits(ModRM(3, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

// TODO : There are special ops for shift by 1, use them

void Emitter::SHLIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0xC1);
	Write8Bits(ModRM(0, 4, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::SHLREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::SHRIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0xC1);
	Write8Bits(ModRM(0, 5, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::SHLREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::SARIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0xC1);
	Write8Bits(ModRM(0, 7, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::SARREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
}

void Emitter::SUBIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 5, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::SUBREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x2B);
	Write8Bits(ModRM(3, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::TSTIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0xF7);
	Write8Bits(ModRM(0, 5, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::XORIMM(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 6, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::XORREG(const UInt16 in_Arg1, const UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x31);
	Write8Bits(ModRM(3, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

/////////////// Control Flow ///////////////

void Emitter::CALL()
{
}

void Emitter::CX()
{
}

void Emitter::RET()
{
	Write8Bits(0xC3);
}

/////////////// Data Transfer ///////////////

void Emitter::MOV(const UInt16 in_Source, const UInt16 in_Dest)
{

}

void Emitter::MOVToStack(const UInt16 in_Source, const UInt16 in_StackOffset)
{
}
