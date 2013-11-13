#include "dynarec.h"

#include <algorithm>

Dynarec::Dynarec(const std::shared_ptr<CPU> & in_CPU) : m_Allocator(16,6), m_CPU(in_CPU), m_Emitter(Emitter()) 
{
	m_EmitTable[0x40] = m_Emitter.ADDIMM;
	m_EmitTable[0x41] = m_Emitter.ADDREG;

	m_EmitTable[0x50] = m_Emitter.SUBIMM;
	m_EmitTable[0x51] = m_Emitter.SUBREG;

	m_EmitTable[0x53] = m_Emitter.CMPIMM;
	m_EmitTable[0x54] = m_Emitter.CMPREG;

	m_EmitTable[0x60] = m_Emitter.ANDIMM;
	m_EmitTable[0x61] = m_Emitter.ANDREG;

	m_EmitTable[0x63] = m_Emitter.TSTIMM;
	// TODO : Deal with TST RX RY
	
	m_EmitTable[0x70] = m_Emitter.ORIMM;
	m_EmitTable[0x71] = m_Emitter.ORREG;
	
	m_EmitTable[0x81] = m_Emitter.XORIMM;
	m_EmitTable[0x82] = m_Emitter.XORREG;

	m_EmitTable[0x91] = m_Emitter.MULIMM;
	m_EmitTable[0x92] = m_Emitter.MULREG;

	m_EmitTable[0xA1] = m_Emitter.DIVIMM;
	m_EmitTable[0xA2] = m_Emitter.DIVREG;

	// TODO : Deal with the shifts
}

Dynarec::~Dynarec() { }

void Dynarec::CompileBasicBlock()
{
	std::vector<Instruction> l_BasicBlock = m_CPU->FetchBasicBlock();

	m_Allocator.AllocateRegisters(l_BasicBlock);

	// Allocate some space on the stack
	m_Emitter.SUBIMM(ESP, 8);

	for(UInt16 i = 0; i < l_BasicBlock.size(); ++i)
	{
		Instruction l_Ins = l_BasicBlock[i];
		LocalAllocator::GPRStatus l_Op1 = m_Allocator.GetPhysicalRegister(l_Ins.GetFirstOperand());
		if(l_Ins.UseImmediateValue())
		{
			if(l_Op1.IsDirty)
			{
				m_Emitter.MOV(l_Op1.PRegID, l_Op1.StackOffset);
			}
			(m_Emitter.*m_EmitTable[l_Ins.GetType()])(l_Op1.PRegID, l_Ins.GetImmediateValue());
			if(l_Op1.IsDirty)
			{
				m_Emitter.MOVToStack(l_Op1.PRegID, l_Op1.StackOffset); 
			}
		}
		else
		{
			LocalAllocator::GPRStatus l_Op2 = m_Allocator.GetPhysicalRegister(l_Ins.GetFirstOperand());
			
			if(l_Ins.IsInplace())
			{
				if(l_Op2.IsDirty)
				{
					m_Emitter.MOV(l_Op2.PRegID, l_Op2.StackOffset);
				}
			}
			else
			{
			}
		}
	}
	m_Emitter.RET();
}

UInt8* Dynarec::ExecuteBlock() const
{
	__asm{
		pushad;
		call 
		popad;
	}
}

