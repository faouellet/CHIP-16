#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <random>
#include <unordered_map>

#include "cpu.h"
#include "gpu.h"
#include "spu.h"

/**
* \class Interpreter
* \brief Interpret Chip16 instructions
*/
class Interpreter
{
private:
	/**
	* \struct LeftShift
	* \brief Functor wrapping the left shift operator
	*/
	struct LeftShift
	{
		UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS)
		{
			return in_LHS << in_RHS;
		}
	};

	/**
	* \struct ArithmeticRightShift
	* \brief Functor wrapping the right shift operator when the value to shift doesn't
	*        have its sign bit on. Otherwise, it uses a mask to copy the sign bit.
	*/
	struct ArithmeticRightShift
	{
		UInt16 operator()(const Int16 & in_LHS, const UInt16 & in_RHS) const
		{
			return in_LHS >> in_RHS;
		}
	};

	/**
	* \struct LogicalRightShift
	* \brief Functor wrapping the right shift operator
	*/
	struct LogicalRightShift
	{
		UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS) const
		{
			return in_LHS >> in_RHS;
		}
	};

private:
	typedef void (Interpreter::*InstructionExec) (const Instruction in_Instruction);
	
private:
	UInt8 m_ErrorCode;				/*!< Current error code */

	std::shared_ptr<CPU> m_CPU;		/*!< The central processing unit */
	GPU m_GPU;						/*!< The graphics processing unit */
	SPU m_SPU;						/*!< The sound processing unit */

	std::mt19937 m_RandEngine;						/*!< Random number engine */
	std::uniform_int_distribution<UInt16> m_Dist;	/*!< Distribution of the random numbers */

	std::unordered_map<UInt8, InstructionExec> m_Ops;	/*!< Interpretations of the opcodes */

public:
	/**
	* \fn Interpreter
	* \brief Default constructor
	* \param in_CPU Pointer to a central processing unit implementation
	*/
	Interpreter(const std::shared_ptr<CPU> & in_CPU = nullptr);
	
	/**
	* \fn ~Interpreter
	* \brief Destructor
	*/
	~Interpreter();

private:
	/**
	* \fn InitOpcodesTable
	* \brief Setup the opcode interpretation table
	*/
	void InitOpcodesTable();

public:
	/**
	* \fn InitDevices
	* \brief Initialize the GPU and the SPU
	* \return An error code
	*/
	unsigned InitDevices();

	/**
	* \fn InterpretOp
	* \brief Read an opcode from the ROM and execute it
	* \return An error code
	*/
	unsigned InterpretOp();

	/**
	* \fn Show
	* \brief Show a frame on screen
	*/
	void Show();

private:	// Arithmetic helpers
	/**
	* \fn BasicArithmetic
	* \brief Apply an instruction to two registers and store the result in a third register
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void BasicArithmetic(const Instruction in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
		std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn DiscardArithmetic
	* \brief Apply an instruction to two registers and discard the result
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void DiscardArithmetic(const Instruction in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
		std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn DiscardImmediateArithmetic
	* \brief Apply an instruction to a register and an immediate value and discard the result
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void DiscardImmediateArithmetic(const Instruction in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
		std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn ImmediateArithmetic
	* \brief Apply an instruction to a register and an immediate value and store the result in the first register
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void ImmediateArithmetic(const Instruction in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins, 
		std::function<void(UInt16,UInt16)> in_FRH);

	/**
	* \fn InplaceArithmetic
	* \brief Apply an instruction to two registers and store the result in the first register
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Ins The instruction to apply 
	* \param in_FRH Handler responsible for updating the flag register
	*/
	void InplaceArithmetic(const Instruction in_Instruction, std::function<UInt16(UInt16,UInt16)> in_Ins,
		std::function<void(UInt16,UInt16)> in_FRH);
	
private: // Instruction helper
	/**
	* \fn FetchHalfByte
	* \brief Fetch half of an instruction byte
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \param in_Pos 0-based position of the half-byte within the instruction starting from the right
	* \return A 4 bits value
	*/
	UInt8 FetchHalfByte(const Instruction in_Instruction, const UInt8 in_Pos) const;

	/**
	* \fn FetchImmediateValue
	* \brief Fetch and concatenate the last two instruction byte
	* \param in_Instruction 4 bytes Chip16 instruction containing the opcode and the operands
	* \return A constant value
	*/
	UInt16 FetchImmediateValue(const Instruction in_Instruction) const;

private:
	 /**
    * \fn InterpretConditions
    * \brief Decode and execute a condition
    * \param in_CondCode A byte containing the condition to be evaluated
    * \return Condition evalution result
    */
    unsigned InterpretConditions(UInt8 in_CondCode);

private:	// Opcodes : See specs for more information
	void ADDI(const Instruction in_Instruction);
	void InplaceADD(const Instruction in_Instruction);
	void ADD(const Instruction in_Instruction);
	void SUBI(const Instruction in_Instruction);
	void InplaceSUB(const Instruction in_Instruction);
	void SUB(const Instruction in_Instruction);
	void CMPI(const Instruction in_Instruction);
	void CMP(const Instruction in_Instruction);
	void ANDI(const Instruction in_Instruction);
	void InplaceAND(const Instruction in_Instruction);
	void AND(const Instruction in_Instruction);
	void TSTI(const Instruction in_Instruction);
	void TST(const Instruction in_Instruction);
	void ORI(const Instruction in_Instruction);
	void InplaceOR(const Instruction in_Instruction);
	void OR(const Instruction in_Instruction);
	void XORI(const Instruction in_Instruction);
	void InplaceXOR(const Instruction in_Instruction);
	void XOR(const Instruction in_Instruction);
	void MULI(const Instruction in_Instruction);
	void InplaceMUL(const Instruction in_Instruction);
	void MUL(const Instruction in_Instruction);
	void DIVI(const Instruction in_Instruction);
	void InplaceDIV(const Instruction in_Instruction);
	void DIV(const Instruction in_Instruction);

	void DirectJMP(const Instruction in_Instruction);
	void Jx(const Instruction in_Instruction);
	void JME(const Instruction in_Instruction);
	void DirectCALL(const Instruction in_Instruction);
	void RET(const Instruction in_Instruction);
	void IndirectJMP(const Instruction in_Instruction);
	void Cx(const Instruction in_Instruction);
	void IndirectCALL(const Instruction in_Instruction);

	void RegisterLDI(const Instruction in_Instruction);
	void StackLDI(const Instruction in_Instruction);
	void DirectLDM(const Instruction in_Instruction);
	void IndirectLDM(const Instruction in_Instruction);
	void MOV(const Instruction in_Instruction);

	void NOP(const Instruction in_Instruction);
	void CLS(const Instruction in_Instruction);
	void VBLNK(const Instruction in_Instruction);
	void BGC(const Instruction in_Instruction);
	void SPR(const Instruction in_Instruction);
	void ImmediateDRW(const Instruction in_Instruction);
	void RegisterDRW(const Instruction in_Instruction);
	void RND(const Instruction in_Instruction);
	void FLIP(const Instruction in_Instruction);
	void SND0(const Instruction in_Instruction);
	void SND1(const Instruction in_Instruction);
	void SND2(const Instruction in_Instruction);
	void SND3(const Instruction in_Instruction);
	void SNP(const Instruction in_Instruction);
	void SNG(const Instruction in_Instruction);

	void ImmediatePalette(const Instruction in_Instruction);
	void RegisterPalette(const Instruction in_Instruction);

	void PUSH(const Instruction in_Instruction);
	void POP(const Instruction in_Instruction);
	void PUSHALL(const Instruction in_Instruction);
	void POPALL(const Instruction in_Instruction);
	void PUSHF(const Instruction in_Instruction);
	void POPF(const Instruction in_Instruction);

	void NSHL(const Instruction in_Instruction);
	void NSHR(const Instruction in_Instruction);
	void NSAR(const Instruction in_Instruction);
	void RegisterSHL(const Instruction in_Instruction);
	void RegisterSHR(const Instruction in_Instruction);
	void RegisterSAR(const Instruction in_Instruction);

	void DirectSTM(const Instruction in_Instruction);
	void IndirectSTM(const Instruction in_Instruction);
};

#endif // INTERPRETER_H
