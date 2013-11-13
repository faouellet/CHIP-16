#ifndef LOCAL_ALLOCATOR_H
#define LOCAL_ALLOCATOR_H

#include "instruction.h"

#include <vector>

/**
* \class LocalAllocator
*/
class LocalAllocator
{	
private:
	typedef std::pair<UInt8, UInt8> RegUsageCount;

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
	* \brief TODO
	*/
	std::vector<UInt8> BlockAnalysis(const std::vector<Instruction> & in_BasicBlock);

	/**
	* \fn AllocateRegisters
	* \brief TODO
	*/
	void AllocateRegisters(std::vector<UInt8> && in_RegIndices);
};

#endif // LOCAL_ALLOCATOR_H
