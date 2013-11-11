#include "dynarec.h"

#include <algorithm>
#include <set>
#include <vector>

Dynarec::Dynarec(const std::shared_ptr<CPU> & in_CPU) : m_CPU(in_CPU), m_Emitter(Emitter()) { }

Dynarec::~Dynarec() { }

void Dynarec::CompileBasicBlock()
{
	std::vector<Instruction> l_BasicBlock(m_CPU->FetchBasicBlock());

	// IDEA : This is where basic optimization of a basic block should go

	std::vector<std::vector<bool>> l_LiveIntervals(AnalyzeRegisterLiveness(l_BasicBlock));
	bool l_RegPool[14];

	for(UInt16 i = 0; i < l_BasicBlock.size(); ++i)
	{
		
	}
}

UInt8* Dynarec::ExecuteBlock() const
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
