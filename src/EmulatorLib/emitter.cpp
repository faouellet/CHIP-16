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

void Emitter::ADDIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x81);
	Write8Bits(ModRM(0, 0, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::ADDREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x03);
	Write8Bits(ModRM(0, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::DIVIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::DIVREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::MULIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::MULREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::SUBIMM(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x80);
	Write8Bits(ModRM(0, 5, DISP32));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::SUBREG(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
	Write8Bits(0x66);
	Write8Bits(0x2B);
	Write8Bits(ModRM(0, in_Arg1, in_Arg2));
	Write16Bits(in_Arg1);
	Write16Bits(in_Arg2);
}

void Emitter::RET()
{
	Write8Bits(0xC3);
}
