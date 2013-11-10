#include "instruction.h"

Instruction::Instruction(const Utils::UInt32 in_Value) : m_Value(in_Value) { }

Instruction::~Instruction() { }

UInt8 Instruction::FetchHalfByte(const UInt8 in_Pos) const
{
	return (m_Value >> (in_Pos*4)) & 0xF;
}

UInt8 Instruction::GetOpcode() const
{
	return m_Value >> 24;
}

UInt8 Instruction::GetFirstOperand() const
{
	return FetchHalfByte(4);
}

UInt8 Instruction::GetSecondOperand() const
{
	return FetchHalfByte(5);
}

UInt8 Instruction::GetThirdOperand() const
{
	return FetchHalfByte(2);
}

UInt16 Instruction::GetImmediateValue() const
{
	UInt16 l_IVal = (m_Value >> 8) & 0xFF;
	return l_IVal | ((m_Value & 0xFF) << 8);
}
