#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "utils.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/**
* \class Instruction
* \brief Abstraction over a Chip16 instruction
*/
class Instruction
{
private:
	UInt32 m_Value;	/*!< 4 byte value of the instruction */
	
	UInt16 m_ImmediateValue;
	
	UInt8 m_Op1;
	UInt8 m_Op2;
	UInt8 m_Op3;
	UInt8 m_Opcode;
	UInt8 m_Type;

	bool m_UseImm;
	bool m_IsInplace;

public:
	/**
	* TODO : Doc
	*/
	enum 
	{
		Misc  = 0x0, 
		Jump  = 0x1, 
		Call  = 0x2, 
		Load  = 0x3, 
		Store = 0x4, 
		Add   = 0x5,
		And   = 0x6,
		Or    = 0x7,
		Xor   = 0x8,
		Mul   = 0x9,
		Div   = 0xA,
		Shift = 0xB,
		Stack = 0xC,
		Pal   = 0xD
	};

public:
	/**
	* \fn Instruction
	* \brief Default constructor
	* \param in_Value Value of the instruction. 0 by default.
	*/
	Instruction(const UInt32 in_Value = 0);
	
	/**
	* \fn Instruction
	* \brief Destructor
	*/
	~Instruction();

private:
	/**
	* \fn FetchHalfByte
	* \brief Fetch half of an instruction byte
	* \param in_Pos 0-based position of the half-byte within the instruction starting from the right
	* \return A 4 bits value
	*/
	UInt8 FetchHalfByte(const UInt8 in_Pos) const;

public: // TODO : Document these functions
	UInt8 GetOpcode() const;
	UInt8 GetFirstOperand() const;
	UInt8 GetSecondOperand() const;
	UInt8 GetThirdOperand() const;
	UInt16 GetImmediateValue() const;
	UInt8 GetType() const;

	bool UseImmediateValue() const;
	bool IsInplace() const;
};

#endif // INSTRUCTION_H
