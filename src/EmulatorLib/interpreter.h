#ifndef INTERPRETER_H
#define INTERPRETER_H

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
	typedef void (Interpreter::*Instruction) (const UInt32 in_Instruction);

private:
	CPU m_CPU;										/*!< The central processing unit */
	GPU m_GPU;										/*!< The graphics processing unit */
	SPU m_SPU;										/*!< The sound processing unit */

	std::mt19937 m_RandEngine;						/*!< Random number engine */
	std::uniform_int_distribution<UInt16> m_Dist;	/*!< Distribution of the random numbers */

	std::unordered_map<UInt8, Instruction> m_Ops;	/*!< Interpretations of the opcodes */

public:
	/**
	* \fn Interpreter
	* \brief Default constructor
	*/
	Interpreter();
	
	/**
	* \fn ~Interpreter
	* \brief Destructor
	*/
	~Interpreter();

public:
	/**
	* \fn InterpretOp
	* \brief Read an opcode from the ROM and execute it
	*/
	unsigned InterpretOp();

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
	
private:
	 /**
    * \fn InterpretConditions
    * \brief Decode and execute a condition
    * \param in_CondCode A byte containing the condition to be evaluated
    * \return Condition evalution result
    */
    unsigned InterpretConditions(UInt8 in_CondCode);

private:	// Opcodes : See specs for more information
	void ADDI(const UInt32 in_Instruction);
	void InplaceADD(const UInt32 in_Instruction);
	void ADD(const UInt32 in_Instruction);
	void SUBI(const UInt32 in_Instruction);
	void InplaceSUB(const UInt32 in_Instruction);
	void SUB(const UInt32 in_Instruction);
	void CMPI(const UInt32 in_Instruction);
	void CMP(const UInt32 in_Instruction);
	void ANDI(const UInt32 in_Instruction);
	void InplaceAND(const UInt32 in_Instruction);
	void AND(const UInt32 in_Instruction);
	void TSTI(const UInt32 in_Instruction);
	void TST(const UInt32 in_Instruction);
	void ORI(const UInt32 in_Instruction);
	void InplaceOR(const UInt32 in_Instruction);
	void OR(const UInt32 in_Instruction);
	void XORI(const UInt32 in_Instruction);
	void InplaceXOR(const UInt32 in_Instruction);
	void XOR(const UInt32 in_Instruction);
	void MULI(const UInt32 in_Instruction);
	void InplaceMUL(const UInt32 in_Instruction);
	void MUL(const UInt32 in_Instruction);
	void DIVI(const UInt32 in_Instruction);
	void InplaceDIV(const UInt32 in_Instruction);
	void DIV(const UInt32 in_Instruction);

	void DirectJMP(const UInt32 in_Instruction);
	void Jx(const UInt32 in_Instruction);
	void JME(const UInt32 in_Instruction);
	void DirectCALL(const UInt32 in_Instruction);
	void RET(const UInt32 in_Instruction);
	void IndirectJMP(const UInt32 in_Instruction);
	void Cx(const UInt32 in_Instruction);
	void IndirectCALL(const UInt32 in_Instruction);

	void RegisterLDI(const UInt32 in_Instruction);
	void StackLDI(const UInt32 in_Instruction);
	void DirectLDM(const UInt32 in_Instruction);
	void IndirectLDM(const UInt32 in_Instruction);
	void MOV(const UInt32 in_Instruction);

	void NOP(const UInt32 in_Instruction);
	void CLS(const UInt32 in_Instruction);
	void VBLNK(const UInt32 in_Instruction);
	void BGC(const UInt32 in_Instruction);
	void SPR(const UInt32 in_Instruction);
	void ImmediateDRW(const UInt32 in_Instruction);
	void RegisterDRW(const UInt32 in_Instruction);
	void RND(const UInt32 in_Instruction);
	void FLIP(const UInt32 in_Instruction);
	void SND0(const UInt32 in_Instruction);
	void SND1(const UInt32 in_Instruction);
	void SND2(const UInt32 in_Instruction);
	void SND3(const UInt32 in_Instruction);
	void SNP(const UInt32 in_Instruction);
	void SNG(const UInt32 in_Instruction);

	void ImmediatePalette(const UInt32 in_Instruction);
	void RegisterPalette(const UInt32 in_Instruction);

	void PUSH(const UInt32 in_Instruction);
	void POP(const UInt32 in_Instruction);
	void PUSHALL(const UInt32 in_Instruction);
	void POPALL(const UInt32 in_Instruction);
	void PUSHF(const UInt32 in_Instruction);
	void POPF(const UInt32 in_Instruction);

	void NSHL(const UInt32 in_Instruction);
	void NSHR(const UInt32 in_Instruction);
	void NSAR(const UInt32 in_Instruction);
	void RegisterSHL(const UInt32 in_Instruction);
	void RegisterSHR(const UInt32 in_Instruction);
	void RegisterSAR(const UInt32 in_Instruction);

	void DirectSTM(const UInt32 in_Instruction);
	void IndirectSTM(const UInt32 in_Instruction);
};

#endif // INTERPRETER_H
