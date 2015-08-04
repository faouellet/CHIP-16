#include "dynarec.h"

std::vector<Instruction> Dynarec::FetchBasicBlock()
{
	Instruction l_Instruction = 0;
	std::vector<Instruction> l_BasicBlock;

	do
	{
		// Q : Should the PC be modified ??
		//l_Instruction = FetchInstruction();

		//l_BasicBlock.push_back(l_Instruction);
	} while ((l_Instruction.GetOpcode() & 0xF0) != 0x10);

	return l_BasicBlock;
}
