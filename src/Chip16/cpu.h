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
	* \brief All possible opcodes
	*/
	enum { NOP=0, CLS, VBLNK, BGC, SPR, DRWXY, DRWXYZ, RND, FLIP00, FLIP01, FLIP10,
			FLIP11, SND0, SND1, SND2, SND3, SNP, SNG, JMP, JX, JME, JMPX, CALL,
			RET, CX, CALLX, LDIX, LDISP, LDMX, LDMXY, MOV, STMX, STMXY, ADDI, 
			ADDXY, ADDXYZ, SUBI, SUBXY, SUBXYZ, CMPI, CMP, ANDI, ANDXY, ANDXYZ,
			TSTI, TST, ORI, ORXY, ORXYZ, XORI, XORXY, XORXYZ, MULI, MULXY, MULXYZ, 
			DIVI, DIVXY, DIVXYZ, SHLXN, SHRXN, SALXN, SARXN, SHLXY, SHRXY, SALXY,
			SARXY, PUSH, POP, PUSHALL, POPALL, PUSHF, POPF, PAL, PALX };

private:
	UInt16 m_PC;							/*!< Program counter */
	UInt16 m_SP;							/*!< Stack pointer */
	UInt16 m_FR;							/*!< Flag register */
	UInt16 m_Registers[16];					/*!< General purpose registers */
	std::function<void()> m_FctTable[73];	/*!< Pointers to every Chip16 instructions */

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
	/**
	* \fn RegisterOpCode
	* \brief Add an emulation function at the opcode index
	* \tparam Opcode		The function opcode
	* \tparam Instruction	The emulation function type
	*/
	template<UInt8 OpCode, class Instruction>
	void RegisterOpCode()
	{
		m_FctTable[OpCode] = [](){ Instruction::Execute(); };
	}

	/**
	* \fn SetNegativeZeroFlag
	* \brief Set the negative flag if the bit[15] of the result is lit
	*        and set the zero flag if the result is zero
	*/
	void SetNegativeZeroFlag(UInt16 in_Result);
	
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
			SetNegativeZeroFlag();
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::divides>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::divides(GetInputRegisterValue(), OpMode::Value());
			// Set carry flag
			m_FR = m_Registers[OpMode::OutAddr()] * OpMode::Value() == GetInputRegisterValue() ?
				m_FR & ~0x2 : m_FR | 0x2;
			SetNegativeZeroFlag();
		}
	};
	
	template<class OpMode>
	struct ArithmeticInstruction<std::minus>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::minus(GetInputRegisterValue(), OpMode::Value());
			// Set carry flag

			SetNegativeZeroFlag();
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::multiplies>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::multiplies(GetInputRegisterValue(), OpMode::Value());
			// Set carry flag

			SetNegativeZeroFlag();
		}
	};

	template<class OpMode>
	struct ArithmeticInstruction<std::plus>
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = std::plus(GetInputRegisterValue(), OpMode::Value());
			// Set carry flag

			SetNegativeZeroFlag();
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
	struct LoadInstruction
	{
		void Execute()
		{
			m_Registers[OpMode::OutAddr()] = OpMode::Value();
		}
	};

	template<class OpMode>
	struct StoreInstruction
	{
		void Execute()
		{
			m_Memory[OpMode::OutAddr()] = OpMode::Value();
		}
	};
};

#endif // CPU_H
