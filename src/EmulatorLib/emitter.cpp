#include "emitter.h"

Emitter::Emitter() : m_Arena(MemoryArena())
{
	m_BufferPtr = reinterpret_cast<Utils::UInt8*>(m_Arena.GetPtr());
}

Emitter::~Emitter()
{
	m_BufferPtr = nullptr;
}
