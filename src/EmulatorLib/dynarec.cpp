#include "dynarec.h"

#include <vector>

Dynarec::Dynarec(const std::shared_ptr<CPU> & in_CPU) : m_CPU(in_CPU), m_Emitter(Emitter()) { }

Dynarec::~Dynarec() { }

void Dynarec::CompileBasicBlock()
{
	std::vector<Instruction> l_BasicBlock(m_CPU->FetchBasicBlock());
	
}

UInt8* Dynarec::ExecuteBlock() const
{
}
