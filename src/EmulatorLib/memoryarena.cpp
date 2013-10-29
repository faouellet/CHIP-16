#include "memoryarena.h"

#if defined(_WIN32)
#include "windows.h"
#else
// TODO : POSIX
#endif

MemoryArena::MemoryArena(unsigned long in_ChunkSize) : m_ChunkSize(in_ChunkSize)
{
#if defined(_WIN32)
	m_Memory = ::VirtualAlloc(0, m_ChunkSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	m_Memory = mmap(0, 0x100000, PROT_READ | PROT_WRITE | PROT_EXEC | MAP_PRIVATE, -1, 0);
#endif
}

MemoryArena::~MemoryArena()
{
#if defined(_WIN32)
	::VirtualFree(m_Memory, m_ChunkSize, MEM_DECOMMIT | MEM_RELEASE);
#else
	munmap(m_Memory, 0x100000);
#endif
}

void* MemoryArena::GetPtr()
{
	return m_Memory;
}
