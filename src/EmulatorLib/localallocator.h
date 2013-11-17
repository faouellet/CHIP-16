#ifndef LOCAL_ALLOCATOR_H
#define LOCAL_ALLOCATOR_H

#include "instruction.h"

#include <vector>

/**
* \class LocalAllocator
* \brief Allocate physical x86 registers to virtual registers given an analysis
*        of the usage of the virtual registers in a basic block. It makes no
*        assumptions concerning the liveness of the virtual registers at the
*        start or at the end of a basic block.
* \author Felix-Antoine Ouellet
*/
class LocalAllocator
{	
public:
	/**
	* \struct GPRStatus
	* \brief Status of a virtual register
	*/
	struct GPRStatus
	{
		bool IsDirty;		/*!< Does the register need to write a value to the stack? */
		UInt8 VRegID;		/*!< ID of the virtual register */
		UInt8 PRegID;		/*!< ID of the associated physical register */
		UInt8 StackOffset;	/*!< Position of the virtual register in the stack */
	};

private:
	UInt8 m_VRegNb;						/*!< Number of virtual registers to work with */
	UInt8 m_PRegNb;						/*!< Number of physical registers to work with */
	std::vector<GPRStatus> m_Registers;	/*!< Status of the virtual registers */

public:
	/*
	* \fn LocalAllocator
	* \brief Default constructor
	* \param in_VRegNb Number of virtual registers to work with
	* \param in_PRegNb Number of physical registers to work with
	*/
	LocalAllocator(const UInt8 in_VRegNb = 0, const UInt8 in_PRegNb = 0);
	
	/*
	* \fn ~LocalAllocator
	* \brief Destructor
	*/
	~LocalAllocator();

public:
	/**
	* \fn AllocateRegisters
	* \brief Map virtual registers contained in a basic block to physical register
	* \param in_BasicBlock Block of sequential instruction
	*/
	void AllocateRegisters(const std::vector<Instruction> & in_BasicBlock);

	/**
	* \fn GetPhysicalRegister
	* \brief Get the physical register associated with the given virtual register.
	*        If no physical register is maaped to the virtual register, it will TODO
	* \param in_VReg A virtual register ID
	* \return Information about the way to access the physical register associated to the virtual register
	*/
	GPRStatus GetPhysicalRegister(const UInt8 in_VReg);

private:
	/**
	* \fn BlockAnalysis
	* \brief Analyze the usage of the virtual registers used in a basic block
	* \param in_BasicBlock Sequence of linear instructions
	* \return Sequence of indices going from the most used virtual register to the least used
	*/
	std::vector<UInt8> BlockAnalysis(const std::vector<Instruction> & in_BasicBlock);

	/**
	* \fn AllocateRegisters
	* \brief Allocate a physical register per virtual register. If there's not
	*        enough physical registers, the top N most used virtual registers will 
	*        get a dedicated physical register and the rest will be flagged as dirty.
	*        Two physical registers will be left free for further use by the virtual
	*        registers wihtout a dedicated physical register.
	* \param List going from the most used virtual register to the least used
	*/
	void AllocateRegisters(std::vector<UInt8> && in_RegIndices);
};

#endif // LOCAL_ALLOCATOR_H
