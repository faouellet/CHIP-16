#include "localallocator.h"

#include <algorithm>

LocalAllocator::LocalAllocator(const UInt8 in_VRegNb, const UInt8 in_PRegNb) : 
	m_VRegNb(in_VRegNb), m_PRegNb(in_PRegNb), m_Registers(m_VRegNb)
{}

LocalAllocator::~LocalAllocator() { }

void LocalAllocator::AllocateRegisters(const std::vector<Instruction> & in_BasicBlock)
{
	AllocateRegisters(BlockAnalysis(in_BasicBlock));
}

LocalAllocator::GPRStatus LocalAllocator::GetPhysicalRegister(const UInt8 in_VReg)
{
	return m_Registers[in_VReg];
}

std::vector<UInt8> LocalAllocator::BlockAnalysis(const std::vector<Instruction> & in_BasicBlock)
{
	std::vector<UInt8> l_UsageCounts(m_VRegNb);

	// TODO : What about and intruction using identical operands ??

	for(UInt16 i = 0; i < in_BasicBlock.size(); ++i)
	{
		++l_UsageCounts[in_BasicBlock[i].GetFirstOperand()];
		if(!in_BasicBlock[i].UseImmediateValue())
		{
			++l_UsageCounts[in_BasicBlock[i].GetSecondOperand()];
			
			if(!in_BasicBlock[i].IsInplace())
			{
				++l_UsageCounts[in_BasicBlock[i].GetThirdOperand()];
			}
		}
	}

	std::vector<UInt8> l_Indices(m_VRegNb);
	for(UInt8 i = 0; i < m_VRegNb; ++i)
		l_Indices[i] = i;

	std::sort(l_Indices.begin(), l_Indices.end(), 
		[&l_UsageCounts](const UInt8 in_Index1, const UInt8 in_Index2)
	{
		return l_UsageCounts[in_Index1] < l_UsageCounts[in_Index2];
	});

	return l_Indices;
}

void LocalAllocator::AllocateRegisters(std::vector<UInt8> && in_RegIndices)
{
	if(m_VRegNb <= m_PRegNb)	// Easy
	{
		for(UInt8 i = 0; i < m_VRegNb; ++i)
		{
			m_Registers[in_RegIndices[i]].PRegID = i;
		}
	}
	else	// NP-Complete
	{
		UInt8 i = 0;
		// Keeping two physical registers free for the lesser used virtual registers
		for(; i < m_PRegNb-2; ++i)
		{
			m_Registers[in_RegIndices[i]].PRegID = i;
		}

		UInt16 l_StackOffset = 0;
		for(UInt8 j = i; j < m_VRegNb; ++j)
		{
			m_Registers[in_RegIndices[j]].IsDirty = true;
			m_Registers[in_RegIndices[j]].StackOffset = l_StackOffset;
			l_StackOffset += 4;
		}
	}
}
