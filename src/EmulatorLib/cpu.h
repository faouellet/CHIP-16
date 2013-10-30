#ifndef CPU_H
#define CPU_H

#include <vector>

#include "SDL.h"

#include "utils.h"

using namespace Utils;

/**
* \class CPU
* \brief Emulates the Central Processing Unit (CPU)
* \author Felix-Antoine Ouellet
*/
class CPU
{
private:
	/**
	* \enum
	* \brief Useful size constants
	*/
	enum { HEADER_SIZE = 16, MEMORY_SIZE = 64*1024, STACK_START = 0xFDF0, CONTROLLER_1 = 0xFFF0, CONTROLLER_2 = 0xFFF2 };

	/**
	* \enum
	* \brief Masks for the flag register
	*/
	enum { UnsignedCarryFlag = 2, UnsignedBorrowFlag = 2, ZeroFlag = 4, 
			SignedOverflowFlag = 64, NegativeFlag = 128 };

	/**
	* \enum
	* \brief Masks for the controllers' bits
	*/
	enum { UP = 1, DOWN = 2, LEFT = 4, RIGHT = 8, SELECT = 16,
			START = 32, A = 64, B = 128 };

private:
	UInt8 m_FR;										/*!< Flag register */
	UInt16 m_PC;									/*!< Program counter */
	UInt16 m_SP;									/*!< Stack pointer */

	UInt16 m_ErrorCode;								/*!< Code used when an error happens during emulation */
	
	UInt16 m_Registers[16];							/*!< General purpose registers */

	UInt8 m_ROMHeader[HEADER_SIZE];					/*!< The header of a .c16 file. See specs for details */
	UInt8 m_Memory[MEMORY_SIZE];					/*!< Memory of the CPU. See specs for layout details */
	
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
	* \fn DumpFlagRegister
	* \brief Dump the flag register
	* \return The content of the flag register
	*/
	UInt16 DumpFlagRegister() const;

	/**
	* \fn DumpMemory
	* \brief Dump the CPU memory
	* \return The data contained in the CPU memory
	*/
	std::vector<UInt8> DumpMemory() const;

	/**
	* \fn DumpProgramCounter
	* \brief Dump the program counter
	* \return The memory address pointed by the PC
	*/
	UInt16 DumpProgramCounter() const;

	/**
	* \fn DumpRegister
	* \brief Dump the value contained in a register
	* \param in_RegID The ID of the register
	* \return The value contained in the register
	*/
	UInt16 DumpRegister(const UInt8 in_RegID) const;

	/**
	* \fn DumpRegisters
	* \brief Dump the CPU registers
	* \return The data contained in the CPU registers
	*/
	std::vector<UInt16> DumpRegisters() const;

	/**
	* \fn DumpStackPointer
	* \brief Dump the stack pointer
	* \return The memory address pointed by the SP
	*/
	UInt16 DumpStackPointer() const;

	/**
	* \fn Init
	* \brief Initialize the central processing unit
	* \param in_ROMData The content of a .c16 binary file
	* \return Error code
	*/
	unsigned Init(std::vector<UInt8> && in_ROMData);

	/**
	* \fn Reset
	* \brief Restore the central processing unit at its pre-initialized state
	*/
	void Reset();

	/**
	* \fn SetProgramCounter
	* \brief Set the value of the PC
	* \param in_Value The new value of the PC
	*/
	void SetProgramCounter(const UInt16 in_Value);

	/**
	* \fn SetRegister
	* \brief Set the value of a register
	* \param in_RegID The ID of the register to update
	* \param in_Value The new value of the register
	*/
	void SetRegister(const UInt8 in_RegID, const UInt16 in_Value);

	/**
	* \fn SetStackPointer
	* \brief Set the value of the SP
	* \param in_Value The new value of the SP
	*/
	void SetStackPointer(const UInt16 in_Value);

	/**
	* \fn UpdateController
	* \brief Update a controller memory mapped IO port
	* \param in_Event The event causing this update
	*/
	void UpdateController(SDL_KeyboardEvent & in_Event);

public:	// Memory helpers
	/**
	* \fn FetchInstruction
	* \brief Read the current instruction and its operands as pointed by the PC.
	*        Note that it increases the PC by 4 i.e. to the next instruction.
	* \return An instruction and its operands
	*/
	UInt32 FetchInstruction();

	/**
	* \fn FetchRegistersValues
	* \brief Extract the values contained within the registers whose addresses
	*        are in the byte pointed by the PC
	*/
	void FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y) const;

	/**
	* \fn FetchSprite
	* \brief Read a number of bytes from memory which corresponds to a sprite data
	* \param in_Addr Address in memory where the sprite is stored
	* \return The sprite data
	*/
	std::vector<UInt8> FetchSprite(UInt16 in_Addr) const;

public:	// Stack helpers
	/**
	* \fn Pop
	* \brief Pop a value from the stack of the emulator and decrement the SP by 2
	* \return The value that was at the top of the stack
	*/
	UInt16 Pop();

	/**
	* \fn Push
	* \brief Push a value on the stack and increment the SP by 2
	* \param in_Val The value to be pushed
	*/
	void Push(UInt16 in_Val);

	/**
	* \fn PushPC
	* \brief Push the PC on the stack and increment the SP by 2
	*/
	void PushPC();

public:	// Flag register helpers
	/**
	* \fn SetSignZeroFlag
	* \brief Set the sign flag if the bit[15] of the result is lit
	*        and set the zero flag if the result is zero
	*/
	void SetSignZeroFlag(UInt16 in_Result);

	/**
	* \fn SetCarryOverflowFlag*
	* \brief Set the carry and the overflow flag depending on the instruction used. 
	* \param in_Op1 The left hand side operand in a computation
	* \param in_Op2 The right hand side operand in a computation
	*/
	void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2);
	void SetCarryOverflowFlagAdd(UInt16 in_Op1, UInt16 in_Op2);
	void SetCarryOverflowFlagDiv(UInt16 in_Op1, UInt16 in_Op2);
	void SetCarryOverflowFlagMul(UInt16 in_Op1, UInt16 in_Op2);
	void SetCarryOverflowFlagSub(UInt16 in_Op1, UInt16 in_Op2);
};

#endif // CPU_H
