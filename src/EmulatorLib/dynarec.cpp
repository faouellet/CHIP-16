#include "dynarec.h"

#include <algorithm>

Dynarec::Dynarec(const std::shared_ptr<CPU> & in_CPU) : m_Allocator(16,6), m_CPU(in_CPU), m_Emitter(Emitter()) 
{
	m_EmitTable[0x40] = &Emitter::ADDIMM;
	m_EmitTable[0x41] = &Emitter::ADDREG;

	m_EmitTable[0x50] = &Emitter::SUBIMM;
	m_EmitTable[0x51] = &Emitter::SUBREG;

	m_EmitTable[0x53] = &Emitter::CMPIMM;
	m_EmitTable[0x54] = &Emitter::CMPREG;

	m_EmitTable[0x60] = &Emitter::ANDIMM;
	m_EmitTable[0x61] = &Emitter::ANDREG;

	m_EmitTable[0x63] = &Emitter::TSTIMM;
	// TODO : Deal with TST RX RY
	
	m_EmitTable[0x70] = &Emitter::ORIMM;
	m_EmitTable[0x71] = &Emitter::ORREG;
	
	m_EmitTable[0x81] = &Emitter::XORIMM;
	m_EmitTable[0x82] = &Emitter::XORREG;

	m_EmitTable[0x91] = &Emitter::MULIMM;
	m_EmitTable[0x92] = &Emitter::MULREG;

	m_EmitTable[0xA1] = &Emitter::DIVIMM;
	m_EmitTable[0xA2] = &Emitter::DIVREG;

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
		if(l_Op1.IsDirty)
		{
			l_Op1.PRegID = EDI;
			m_Emitter.MOV(l_Op1.PRegID, l_Op1.StackOffset);
		}

		if(l_Ins.UseImmediateValue())
		{
			(m_Emitter.*m_EmitTable[l_Ins.GetType()])(l_Op1.PRegID, l_Ins.GetImmediateValue());
		}
		else
		{
			LocalAllocator::GPRStatus l_Op2 = m_Allocator.GetPhysicalRegister(l_Ins.GetSecondOperand());
			
			if(l_Op2.IsDirty)
			{
				l_Op2.PRegID = ESI;
				m_Emitter.MOV(l_Op2.PRegID, l_Op2.StackOffset);
			}
			
			if(l_Ins.IsInplace())
			{	
				(m_Emitter.*m_EmitTable[l_Ins.GetType()])(l_Op1.PRegID, l_Op2.PRegID);
			}
			else
			{
				LocalAllocator::GPRStatus l_Op3 = m_Allocator.GetPhysicalRegister(l_Ins.GetThirdOperand());

				if(l_Op3.IsDirty)
				{
					(m_Emitter.*m_EmitTable[l_Ins.GetType()])(l_Op1.PRegID, l_Op2.PRegID);
					m_Emitter.MOVToStack(l_Op1.PRegID, l_Op3.StackOffset);
				}
				else
				{
					m_Emitter.MOV(l_Op3.PRegID, l_Op1.PRegID);
					(m_Emitter.*m_EmitTable[l_Ins.GetType()])(l_Op3.PRegID, l_Op2.PRegID);
				}
			}
		}

		if(l_Op1.IsDirty)
			m_Emitter.MOVToStack(EDI, l_Op1.StackOffset);
	}
	m_Emitter.RET();
}

void Dynarec::ExecuteBlock() const
{
	// TODO : Well shit, this is not portable
	/*__asm{
		pushad;
		call 
		popad;
	}*/
}

