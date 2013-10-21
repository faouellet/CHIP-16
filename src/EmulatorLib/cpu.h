#ifndef CPU_H
#define CPU_H

#include <functional>
#include <map>
#include <memory>
#include <random>
#include <vector>

#include "gpu.h"
#include "spu.h"

using namespace Utils;

/**
* \class CPU
* \brief Emulates the Central Processing Unit (CPU)
* \author Felix-Antoine Ouellet
*/
class CPU
{
private:
	typedef void (CPU::*Instruction) ();

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

	GPU m_GPU;										/*!< The graphics processing unit */
	SPU m_SPU;										/*!< The sound processing unit */

	std::mt19937 m_RandEngine;						/*!< Random number engine */
	std::uniform_int_distribution<UInt16> m_Dist;	/*!< Distribution of the random numbers */

	std::map<UInt8, Instruction> m_Ops;	/*!< Interpretations of the opcodes */

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
	* \fn FlushGPU
	* \brief Flush the screen buffer of the GPU
	*/
	void FlushGPU();

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
	* \fn InterpretOp
	* \brief Read an opcode from the ROM and execute it
	*/
	unsigned InterpretOp();

	/**
	* \fn Reset
	* \brief Restore the central processing unit at its pre-initialized state
	*/
	void Reset();

	/**
	* \fn UpdateController
	* \brief Update a controller memory mapped IO port
	* \param in_Event The event causing this update
	*/
	void UpdateController(SDL_KeyboardEvent & in_Event);

private:	// Arithmetic helpers
	/**
	* \fn BasicArithmetic
	* \brief Apply an instruction to two registers and store the result in a third register
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void BasicArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn DiscardArithmetic
	* \brief Apply an instruction to two registers and discard the result
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void DiscardArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn DiscardImmediateArithmetic
	* \brief Apply an instruction to a register and an immediate value and discard the result
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void DiscardImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn ImmediateArithmetic
	* \brief Apply an instruction to a register and an immediate value and store the result in the first register
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void ImmediateArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn InplaceArithmetic
	* \brief Apply an instruction to two registers and store the result in the first register
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void InplaceArithmetic(std::function<UInt16(UInt16,UInt16)> in_Ins, std::function<void(UInt16,UInt16)> in_FRH);
	
private:	// Memory helpers
	/**
	* \fn FetchImmediateValue
	* \brief Concatenate the value pointed by the PC with the one the PC is going
	*        to point next. Note that this function will increment the PC by 2.
	* \return The immediate value at the end of the opcode
	*/
	UInt16 FetchImmediateValue();

	/**
	* \fn FetchRegisterAddress
	* \brief Extract the address given at where the PC is pointed.
	*        This function increment the PC by 1.
	* \return A register address
	*/
	UInt16 FetchRegisterAddress();

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

private:	// Stack helpers
	/**
	* \fn Pop
	* \brief Pop a value from the stack of the emulator and decrement the SP by 2
	* \return The value that was at the top of the stack
	*/
	UInt16 Pop();

	/**
	* \fn Push
	* \brief Push a value on the stack of the emulator and increment the SP by 2
	* \param in_Val The value to be pushed
	*/
	void Push(UInt16 in_Val);

private:	// Flag register helpers
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

private:
	 /**
    * \fn InterpretConditions
    * \brief Decode and execute a condition
    * \param in_CondCode A byte containing the condition to be evaluated
    * \return Condition evalution result
    */
    unsigned InterpretConditions(UInt8 in_CondCode);

private:	// Opcodes : See specs for more information
	void ADDI();
	void InplaceADD();
	void ADD();
	void SUBI();
	void InplaceSUB();
	void SUB();
	void CMPI();
	void CMP();
	void ANDI();
	void InplaceAND();
	void AND();
	void TSTI();
	void TST();
	void ORI();
	void InplaceOR();
	void OR();
	void XORI();
	void InplaceXOR();
	void XOR();
	void MULI();
	void InplaceMUL();
	void MUL();
	void DIVI();
	void InplaceDIV();
	void DIV();

	void DirectJMP();
	void Jx();
	void JME();
	void DirectCALL();
	void RET();
	void IndirectJMP();
	void Cx();
	void IndirectCALL();

	void RegisterLDI();
	void StackLDI();
	void DirectLDM();
	void IndirectLDM();
	void MOV();

	void NOP();
	void CLS();
	void VBLNK();
	void BGC();
	void SPR();
	void ImmediateDRW();
	void RegisterDRW();
	void RND();
	void FLIP();
	void SND0();
	void SND1();
	void SND2();
	void SND3();
	void SNP();
	void SNG();

	void ImmediatePalette();
	void RegisterPalette();

	void PUSH();
	void POP();
	void PUSHALL();
	void POPALL();
	void PUSHF();
	void POPF();

	void NSHL();
	void NSHR();
	void NSAR();
	void RegisterSHL();
	void RegisterSHR();
	void RegisterSAR();

	void DirectSTM();
	void IndirectSTM();
};

#endif // CPU_H
