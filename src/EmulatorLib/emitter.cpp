#include "emitter.h"

Emitter::Emitter() : m_Arena(MemoryArena())
{
	m_BufferPtr = reinterpret_cast<Utils::UInt8*>(m_Arena.GetPtr());
}

Emitter::~Emitter()
{
	m_BufferPtr = nullptr;
}

void Emitter::ADD(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::DIV(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::MUL(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}

void Emitter::SUB(const Utils::UInt16 in_Arg1, const Utils::UInt16 in_Arg2)
{
}
