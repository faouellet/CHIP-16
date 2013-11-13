//#include "globalallocator.h"
//
//Utils::UInt8 GlobalAllocator::GetAvailableRegister()
//{
//	UInt8 l_Reg;
//
//	if(m_FreeNRegs.begin() != m_FreeNRegs.end())
//	{
//		l_Reg = *m_FreeNRegs.begin();
//		m_FreeNRegs.erase(m_FreeNRegs.begin());
//	}
//	else // We need to spill a register to memory
//	{
//		// Using a least soon used heuristic to find a suitable native register
//		Int16 l_NextUsage[6] = { -1, -1, -1, -1, -1, -1 };
//		for(UInt16 i = m_CurrentInsIndex+1; i < m_BasicBlock.size(); ++i)
//		{
//			if(m_LiveIntervals[i]
//		}
//	}
//
//	return l_Reg;
//}
//
//void GlobalAllocator::AnalyzeRegisterLiveness()
//{
//	std::vector<std::set<UInt8>> l_Def;
//	std::vector<std::set<UInt8>> l_Use;
//
//	// Construct the Def and Use set
//	for(UInt16 i = 0; i < m_BasicBlock.size(); ++i)
//	{
//		Instruction l_Instruction = m_BasicBlock[i];
//		UInt8 l_Opcode = l_Instruction.GetOpcode();
//
//		l_Use[i].insert(l_Instruction.GetFirstOperand());
//
//		if(l_Instruction.GetType() == Instruction::Shift)
//			if((l_Opcode & 0xF) > 2)
//				l_Use[i].insert(l_Instruction.GetSecondOperand());
//		else if(l_Opcode & 0xF)
//			l_Use[i].insert(l_Instruction.GetSecondOperand());
//
//		if((l_Opcode & 0xF) == 2)
//			l_Def[i].insert(l_Instruction.GetThirdOperand());
//	}
//
//	std::vector<std::set<UInt8>> l_LiveIn;
//	std::vector<std::set<UInt8>> l_LiveOut;
//
//	std::vector<std::set<UInt8>> l_NewLiveIn;
//	std::vector<std::set<UInt8>> l_NewLiveOut;
//
//	std::set<UInt8> l_Diff;
//
//	// Construct the In and Out set
//	do
//	{
//		for(Int16 i = m_BasicBlock.size()-1; i > -1; --i)
//		{
//			l_NewLiveIn[i] = l_LiveIn[i];
//			l_NewLiveOut[i] = l_LiveOut[i];
//
//			std::set_difference(l_LiveOut[i].begin(), l_LiveOut[i].end(), 
//				l_Def[i].begin(), l_Def[i].end(), l_Diff.begin());
//
//			std::set_union(l_Use[i].begin(), l_Use[i].end(), l_Diff.begin(), l_Diff.end(), 
//				l_LiveIn[i].begin());
//
//			if(i < m_BasicBlock.size()-1)
//				l_LiveOut[i] = l_LiveIn[i]; 
//		}
//	}
//	while((l_NewLiveIn != l_LiveIn) && (l_NewLiveOut != l_LiveOut));
//
//	// Construct de the live intervals
//	m_LiveIntervals = std::vector<std::vector<std::pair<UInt16,UInt16>>>(6, std::vector<std::pair<UInt16,UInt16>>());
//
//	for(UInt16 i = 0; i < m_BasicBlock.size(); ++i)
//	{
//
//		// Q : Should l_LiveOut be used here ??
//	}
//}