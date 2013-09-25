#ifndef CPU_H
#define CPU_H

#include <functional>
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
	* \brief Useful size constants
	*/
	enum { OPCODE_SIZE = 4, HEADER_SIZE = 16, MEMORY_SIZE = 65536 };

	/**
	* \enum
	* \brief Masks for the flag register
	*/
	enum { UnsignedCarry = 1, UnsignedBorrow = 1, Zero = 2, SignedOverflow = 64, Sign = 128 };

private:
	UInt16 m_PC;							/*!< Program counter */
	UInt16 m_SP;							/*!< Stack pointer */
	UInt8 m_FR;								/*!< Flag register */
	UInt16 m_Registers[16];					/*!< General purpose registers */
	std::function<void()> m_FctTable[42];	/*!< Pointers to every Chip16 instructions */

	// Q : Maybe change to vector ?
	UInt8 m_ROMHeader[HEADER_SIZE];	/*!< The header of a .c16 file. See specs for details*/
	UInt8 m_Memory[MEMORY_SIZE];	/*!< Memory of the CPU. See specs for layout details */

private:
	template<int RegNo>	// Q : Could RegNo be tested to see if it's not >16 ??
	struct Register
	{
		static UInt16 Get() { return m_Registers[RegNo]; }
		static void Set(const UInt16 in_Val) { m_Registers[RegNo] = in_Val; }
	};

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
	bool Init(std::vector<UInt8> && in_ROMData);

	/**
	* \fn InterpretOp
	* \n brief Read an opcode from the ROM and execute it
	*/
	void InterpretOp();

private:
	/**
	* \fn FetchOpCode
	* \brief Read 4 bytes from the ROM data and increments the program counter accordingly
	* \return Opcode corresponding to what was read from the ROM
	*/
	UInt32 FetchOpCode();

// Template metaprogramming goodness inspired by http://vermeille.fr/dotclear2/index.php/post/23-Metaprogramming-a-full-Virtual-Machine
private:
	/*
	* \struct Instruction
	* \brief Static abstraction of the execution of an instruction
	*/
	template<template<class,class,class, class> Ins, class Fct, class Op1, class Op2, class Op3>
	struct Instruction 
	{
		static void Execute()
		{
			Ins<Fct, Op1, Op2, Op3>::Execute();
		}
	};

	template<UInt8 OpCode, class Instruction>
	void RegisterOpCode()
	{
		m_FctTable[OpCode] = [](){ Instruction::Execute(); };
	}

private:	// Specific instruction type definition
	template<class Fct, class Op1, class Op2, class Op3>
	struct ArithmeticInstruction
	{
		static void Execute()
		{
			Op3::Set(Fct(Op1::Get(),Op2::Get()));
		}
	};
};

#endif // CPU_H
