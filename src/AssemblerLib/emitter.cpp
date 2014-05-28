#include "emitter.h"

#include <fstream>
#include <iostream>

Emitter::Emitter(const UInt32 & in_BufSize) :
    m_Buffer(in_BufSize / 4 + 1) { }

Emitter::~Emitter() { }

void Emitter::EmitInstruction(const UInt8 in_Opcode, const UInt8 in_Op1, 
        const UInt8 in_Op2, const UInt8 in_Op3)
{
    UInt32 l_Ins = in_Opcode;
    l_Ins = l_Ins << 8 | in_Op2;
    l_Ins = l_Ins << 4 | in_Op1;
    l_Ins = l_Ins << 8 | in_Op3;
    m_Buffer.push_back(l_Ins);
}

void Emitter::EmitToFile(const std::string & in_Filename) const
{
	std::ofstream l_Stream(in_Filename);

	if (!l_Stream.is_open())
	{

	}

    // TODO: Chip16 Header
    
}

/////////////// Arithmetric ///////////////

void Emitter::EmitAdd(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x42, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitAddImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x40, in_Op1, 0, in_Op2);
}

void Emitter::EmitAddInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x41, in_Op1, in_Op2, 0);
}

void Emitter::EmitAnd(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x62, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitAndImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x60, in_Op1, 0, in_Op2);
}

void Emitter::EmitAndInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x61, in_Op1, in_Op2, 0);
}

void Emitter::EmitCmp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x54, in_Op1, in_Op2, 0);
}

void Emitter::EmitARShift(const UInt8 in_Op1, const UInt8 in_Op2)
{

    EmitInstruction(0xB5, in_Op1, in_Op2, 0);
}

void Emitter::EmitARShiftImm(const UInt8 in_Op1, const UInt8 in_Op2)
{

    EmitInstruction(0xB2, in_Op1, 0, in_Op2);
}

void Emitter::EmitCmpImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x53, in_Op1, 0, in_Op2);
}

void Emitter::EmitDiv(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0xA2, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitDivImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xA0, in_Op1, 0, in_Op2);
}

void Emitter::EmitDivInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xA1, in_Op1, in_Op2, 0);
}

void Emitter::EmitLLShift(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xB3, in_Op1, in_Op2, 0);
}

void Emitter::EmitLRShift(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xB4, in_Op1, in_Op2, 0);
}

void Emitter::EmitLLShiftImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xB0, in_Op1, 0, in_Op2);
}

void Emitter::EmitLRShiftImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0xB1, in_Op1, 0, in_Op2);
}

void Emitter::EmitMul(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x92, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitMulImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x90, in_Op1, 0, in_Op2);
}

void Emitter::EmitMulInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x91, in_Op1, in_Op2, 0);
}

void Emitter::EmitOr(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x72, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitOrImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x70, in_Op1, 0, in_Op2);
}

void Emitter::EmitOrInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x71, in_Op1, in_Op2, 0);
}

void Emitter::EmitSub(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x52, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitSubImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x50, in_Op1, 0, in_Op2);
}

void Emitter::EmitSubInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x51, in_Op1, in_Op2, 0);
}

void Emitter::EmitTst(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x64, in_Op1, 0, in_Op2);
}

void Emitter::EmitTstImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x63, in_Op1, in_Op2, 0);
}

void Emitter::EmitXor(const UInt8 in_Op1, const UInt8 in_Op2, const UInt8 in_Op3)
{
    EmitInstruction(0x82, in_Op1, in_Op2, in_Op3);
}

void Emitter::EmitXorImm(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x80, in_Op1, 0, in_Op2);
}

void Emitter::EmitXorInp(const UInt8 in_Op1, const UInt8 in_Op2)
{
    EmitInstruction(0x81, in_Op1, in_Op2, 0);
}

/////////////// Stack ///////////////

void Emitter::EmitPush(const UInt8 in_RegID)
{
    EmitInstruction(0xC0, in_RegID, 0, 0);
}

void Emitter::EmitPop(const UInt8 in_RegID)
{
    EmitInstruction(0xC1, in_RegID, 0, 0);
}

void Emitter::EmitPushAll()
{
    EmitInstruction(0xC2, 0, 0, 0);
}

void Emitter::EmitPopAll()
{
    EmitInstruction(0xC3, 0, 0, 0);
}

void Emitter::EmitPushF()
{
    EmitInstruction(0xC4, 0, 0, 0);
}

void Emitter::EmitPopF()
{
    EmitInstruction(0xC5, 0, 0, 0);
}

