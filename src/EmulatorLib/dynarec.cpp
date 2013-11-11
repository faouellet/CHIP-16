#include "dynarec.h"

#include <algorithm>
#include <set>
#include <vector>

Dynarec::Dynarec(const std::shared_ptr<CPU> & in_CPU) : m_CPU(in_CPU), m_Emitter(Emitter()) 
{
	memset(m_RegUsed, false, sizeof(bool)*16);

	m_EmitTable[0x40] = m_Emitter.ADDIMM;
	m_EmitTable[0x41] = m_Emitter.ADDREG;

	m_EmitTable[0x50] = m_Emitter.SUBIMM;
	m_EmitTable[0x51] = m_Emitter.SUBREG;

	m_EmitTable[0x53] = m_Emitter.CMPIMM;
	m_EmitTable[0x54] = m_Emitter.CMPREG;

	m_EmitTable[0x60] = m_Emitter.ANDIMM;
	m_EmitTable[0x61] = m_Emitter.ANDREG;

	m_EmitTable[0x63] = m_Emitter.TSTIMM;
	//m_EmitTable[0x64] = m_Emitter.;
	
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
	std::vector<Instruction> l_BasicBlock(m_CPU->FetchBasicBlock());

	// IDEA : This is where peephole optimization of a basic block should go

	std::vector<std::vector<bool>> l_LiveIntervals(AnalyzeRegisterLiveness(l_BasicBlock));
	Instruction l_Instruction;
	UInt8 l_Opcode;

	for(UInt16 i = 0; i < l_BasicBlock.size(); ++i)
	{
		l_Instruction = l_BasicBlock[i]; 
		l_Opcode = l_Instruction.GetOpcode();

		// TODO : Deal with the shifts
		if((l_Opcode & 0xF) == 2)
		{
			// TODO : Deal with 3 registers instructions
		}
		else
		{
			UInt8 l_Op1Reg = GetAvailableRegister();
			m_VRegsToNRegs[l_Instruction.GetFirstOperand()] = l_Op1Reg;
			
			UInt8 l_Op2Reg = l_Op1Reg;
			if(!l_Instruction.UseImmediateValue())
			{
				l_Op2Reg = GetAvailableRegister();
				m_VRegsToNRegs[l_Instruction.GetSecondOperand()] = l_Op2Reg;
			}

			(m_Emitter.*m_EmitTable[l_Instruction.GetOpcode()])(l_Op1Reg, l_Op1Reg);
		}

		// Free dead registers
		for(UInt8 j = 0; j < 16; ++j)
		{

		}

	}
	m_Emitter.RET();
}

UInt8* Dynarec::ExecuteBlock() const
{
}

UInt8 Dynarec::GetAvailableRegister()
{

}

std::vector<std::vector<bool>> Dynarec::AnalyzeRegisterLiveness(const std::vector<Instruction> & in_BasicBlock) const
{
	std::vector<std::set<UInt8>> l_Def;
	std::vector<std::set<UInt8>> l_Use;

	// Construct the Def and Use set
	for(UInt16 i = 0; i < in_BasicBlock.size(); ++i)
	{
		Instruction l_Instruction = in_BasicBlock[i];
		UInt8 l_Opcode = l_Instruction.GetOpcode();

		l_Use[i].insert(l_Instruction.GetFirstOperand());

		if(l_Instruction.GetType() == Instruction::Shift)
			if((l_Opcode & 0xF) > 2)
				l_Use[i].insert(l_Instruction.GetSecondOperand());
		else if(l_Opcode & 0xF)
			l_Use[i].insert(l_Instruction.GetSecondOperand());

		if((l_Opcode & 0xF) == 2)
			l_Def[i].insert(l_Instruction.GetThirdOperand());
	}

	std::vector<std::set<UInt8>> l_LiveIn;
	std::vector<std::set<UInt8>> l_LiveOut;

	std::vector<std::set<UInt8>> l_NewLiveIn;
	std::vector<std::set<UInt8>> l_NewLiveOut;

	std::set<UInt8> l_Diff;

	// Construct the In and Out set
	do
	{
		for(Int16 i = in_BasicBlock.size()-1; i > -1; --i)
		{
			l_NewLiveIn[i] = l_LiveIn[i];
			l_NewLiveOut[i] = l_LiveOut[i];

			std::set_difference(l_LiveOut[i].begin(), l_LiveOut[i].end(), 
				l_Def[i].begin(), l_Def[i].end(), l_Diff.begin());

			std::set_union(l_Use[i].begin(), l_Use[i].end(), l_Diff.begin(), l_Diff.end(), 
				l_LiveIn[i].begin());

			if(i < in_BasicBlock.size()-1)
				l_LiveOut[i] = l_LiveIn[i]; 
		}
	}
	while((l_NewLiveIn != l_LiveIn) && (l_NewLiveOut != l_LiveOut));

	// Construct de the live intervals
	std::vector<std::vector<bool>> l_LiveIntervals(in_BasicBlock.size(), std::vector<bool>(16, false));

	for(UInt16 i = 0; i < in_BasicBlock.size(); ++i)
	{
		for(auto l_LiveVar : l_LiveIn)
			l_LiveIntervals[i][l_LiveVar] = true;

		// Q : Should l_LiveOut be used here ??
	}

	return l_LiveIntervals;
}
