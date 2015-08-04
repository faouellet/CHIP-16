#ifndef DYNAREC_H
#define DYNAREC_H

#include "instruction.h"

#include <vector>

/**
* \class Dynarec
* \brief TODO
*/
class Dynarec
{
public:
	Dynarec() = default;
	~Dynarec() = default;

private:
	/**
	* \fn FetchBasicBlock
	* \brief Fetch a contiguous block of sequential instruction
	* \return A block of instructions
	*/
	std::vector<Instruction> FetchBasicBlock();
};

#endif // DYNAREC_H
