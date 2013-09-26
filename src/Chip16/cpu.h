#ifndef CPU_H
#define CPU_H

#include <functional>
#include <vector>

#include "utils.h"

using namespace Utils;

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
	std::function<void()> m_FctTable[75];	/*!< Pointers to every Chip16 instructions */

	// Q : Maybe change to vector ?
	UInt8 m_ROMHeader[HEADER_SIZE];	/*!< The header of a .c16 file. See specs for details*/
	UInt8 m_Memory[MEMORY_SIZE];	/*!< Memory of the CPU. See specs for layout details */

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
	* \fn Run
	* \n brief Read an opcode from the ROM and execute it
	*/
	void Run();

private:
	/**
	* \fn FetchOpCode
	* \brief Read 4 bytes from the ROM data and increments the program counter accordingly
	* \return Opcode corresponding to what was read from the ROM
	*/
	UInt32 FetchOpCode();

	/**
	* \fn GetInputRegisterValue
	* \brief Read the index of the register pointed by the PC and get the 
	*        content of the given register
	* \return Opcode corresponding to what was read from the ROM
	*/
	UInt16 GetInputRegisterValue();

private:
	template<UInt8 OpCode, class Instruction>
	void RegisterOpCode()
	{
		m_FctTable[OpCode] = [](){ Instruction::Execute(); };
	}
	
private:
	struct ImmediateOpMode 
	{
		static void OutAddr() { }
		static void Value() { }
	};

	struct IndirectOpMode
	{
		static void OutAddr() { }
		static void Value() { }
	};

	struct InplaceOpMode
	{
		static void OutAddr() { }
		static void Value() { }
	};

	struct StandardOpMode
	{
		static void OutAddr() { }
		static void Value() { }
	};

private:
	template<template<class> Fct, class OpMode>
	struct ArithmeticInstruction 
	{
		void Execute()
		{
			Fct<UInt16> l_Func; // Q : Could there be a better (static) way (except constexpr functor) ?
			m_Registers[OpMode::OutAddr()] = l_Func(GetInputRegisterValue(), OpMode::Value());
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::divides>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::divides(GetInputRegisterValue(), OpMode::Value());
		}
	};
	
	template<class OpMode>
	struct ArithmeticInstruction<std::minus>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::minus(GetInputRegisterValue(), OpMode::Value());
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::multiplies>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::multiplies(GetInputRegisterValue(), OpMode::Value());
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::plus>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::plus(GetInputRegisterValue(), OpMode::Value());
		}
	};

	template<class Cond>
	struct JumpInstruction 
	{
		void Execute()
		{
			m_PC = Cond::GetAddr();
		}
	};

	template<class OpMode>
	struct LoadStoreInstruction
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = OpMode::Value();
		}
	};
};

#endif // CPU_H
