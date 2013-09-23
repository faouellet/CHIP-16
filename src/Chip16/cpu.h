#ifndef CPU_H
#define CPU_H

#include <vector>

#include "utils.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/*
* \class CPU
* \brief Emulates the Central Processing Unit (SPU)
* \author Félix-Antoine Ouellet
* \version 0.1
*/
class CPU
{
private:
	/**
	* \enum
	* \brief Useful siz constants
	*/
	enum { OPCODE_SIZE = 4, HEADER_SIZE = 16, MEMORY_SIZE = 65536 };

private:
	UInt16 m_PC;		/*!< Program counter */
	UInt16 m_SP;		/*!< Stack pointer */
	UInt8 m_FR;			/*!< Flag register */
	UInt16 m_Regs[16];	/*!< General purpose registers */

	// Q : Maybe change to vector ?
	UInt8 m_ROMHeader[HEADER_SIZE];	/*!< The header of a .c16 file. See specs for details*/
	UInt8 m_Memory[MEMORY_SIZE];	/*!< Memory of the CPU. See specs for layout details */

private:
	/**
	* \enum
	* \brief Masks for the flag register
	*/
	enum { UnsignedCarry = 1, UnsignedBorrow = 1, Zero = 2, SignedOverflow = 64, Sign = 128 };

public:
	/**
	* \fn CPU
	* \brief Default constructor
	*/
	CPU();

	/**
	* \fn ~CPU
	* \brief Destructor
	*/
	~CPU();

public:
	/**
	* \fn Init
	* \brief Initialize the central processing unit
	* \param in_ROMData The content of a .c16 binary file
	* \return Success or failure
	*/
	bool Init(std::vector<const UInt8> && in_ROMData);

	/**
	* \fn InterpretOp
	* \n brief Read an opcode from the ROM and execute it
	*/
	void InterpretOp();

private:
	/**
	* \fn InterpretArithmetics
	* \brief Decode and execute an arithmetic opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretArithmetics(UInt32 && in_OpCode);

	/**
	* \fn InterpretCalls
	* \brief Decode and execute an call opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretCalls(UInt32 && in_OpCode);

	/**
	* \fn InterpretJumps
	* \brief Decode and execute an jump opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretJumps(UInt32 && in_OpCode);

	/**
	* \fn InterpretLoads
	* \brief Decode and execute an load opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretLoads(UInt32 && in_OpCode);

	// TODO : Find a better name
	/**
	* \fn InterpretMisc
	* \brief Decode and execute an misc opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretMisc(UInt32 && in_OpCode);

	/**
	* \fn InterpretPushPops
	* \brief Decode and execute an push or pop opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretPushPops(UInt32 && in_OpCode);

	/**
	* \fn InterpretStores
	* \brief Decode and execute an store  opcode
	* \param in_OpCode A 4 bytes Chip16 instruction
	*/
	void InterpretStores(UInt32 && in_OpCode);

	/**
	* \fn FetchOpCode
	* \brief Read 4 bytes from the ROM data and increments the program counter accordingly
	* \return Opcode corresponding to what was read from the ROM
	*/
	UInt32 FetchOpCode();
};

#endif // SPU_H
