#include "emitter.h"

#include <fstream>
#include <iostream>

Emitter::Emitter(const UInt32 & in_BufSize) :
    m_Buffer(in_BufSize / 4 + 1) { }

Emitter::~Emitter() { }

void Emitter::EmitInstruction(const UInt8 in_Opcode, const UInt8 in_Op1, 
        const UInt8 in_Op2, const UInt16 in_Op3)
{
    UInt32 l_Ins = in_Opcode;
    l_Ins = l_Ins << 4 | in_Op2;
    l_Ins = l_Ins << 4 | in_Op1;
    l_Ins = l_Ins << 16 | in_Op3;
    m_Buffer.push_back(l_Ins);
}

UInt16 Emitter::ReverseBytes(const UInt16 in_Bytes)
{
	return UInt16(((in_Bytes & 0xFF) << 8) | (in_Bytes >> 8));
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

void Emitter::EmitAdd(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x42, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitAddImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x40, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitAddInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x41, in_RegX, in_RegY, 0);
}

void Emitter::EmitAnd(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x62, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitAndImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x60, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitAndInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x61, in_RegX, in_RegY, 0);
}

void Emitter::EmitARShift(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0xB5, in_RegX, in_RegY, 0);
}

void Emitter::EmitARShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal)
{

	EmitInstruction(0xB2, in_RegX, 0, UInt16(in_ImmVal) << 8);
}

void Emitter::EmitCmp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x54, in_RegX, in_RegY, 0);
}

void Emitter::EmitCmpImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x53, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitDiv(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0xA2, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitDivImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0xA0, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitDivInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0xA1, in_RegX, in_RegY, 0);
}

void Emitter::EmitLLShift(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0xB3, in_RegX, in_RegY, 0);
}

void Emitter::EmitLLShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal)
{
	EmitInstruction(0xB0, in_RegX, 0, UInt16(in_ImmVal) << 8);
}

void Emitter::EmitLRShift(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0xB4, in_RegX, in_RegY, 0);
}

void Emitter::EmitLRShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal)
{
	EmitInstruction(0xB1, in_RegX, 0, UInt16(in_ImmVal) << 8);
}

void Emitter::EmitMul(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x92, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitMulImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x90, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitMulInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x91, in_RegX, in_RegY, 0);
}

void Emitter::EmitOr(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x72, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitOrImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x70, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitOrInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x71, in_RegX, in_RegY, 0);
}

void Emitter::EmitSub(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x52, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitSubImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x50, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitSubInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x51, in_RegX, in_RegY, 0);
}

void Emitter::EmitTst(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x64, in_RegX, in_RegY, 0);
}

void Emitter::EmitTstImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x63, in_RegX, ReverseBytes(in_ImmVal), 0);
}

void Emitter::EmitXor(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x82, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitXorImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x80, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitXorInp(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x81, in_RegX, in_RegY, 0);
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

/////////////// Memory ///////////////
void Emitter::EmitLDIImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x20, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitLDISP(const UInt16 in_ImmVal)
{
	EmitInstruction(0x21, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitLDM(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x23, in_RegX, in_RegY, 0);
}

void Emitter::EmitLDMImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x22, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitMov(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x24, in_RegX, in_RegY, 0);
}

void Emitter::EmitStm(const UInt8 in_RegX, const UInt8 in_RegY)
{
	EmitInstruction(0x31, in_RegX, in_RegY, 0);
}

void Emitter::EmitStmImm(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x30, in_RegX, 0, ReverseBytes(in_ImmVal));
}

/////////////// Jump ///////////////
void Emitter::EmitJmp(const UInt16 in_ImmVal)
{
	EmitInstruction(0x10, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitJx(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x12, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitJme(const UInt8 in_RegX, const UInt8 in_RegY, const UInt16 in_ImmVal)
{
	EmitInstruction(0x13, in_RegX, in_RegY, ReverseBytes(in_ImmVal));
}

void Emitter::EmitJmpInd(const UInt8 in_RegX)
{
	EmitInstruction(0x16, in_RegX, 0, 0);
}

void Emitter::EmitCall(const UInt8 in_RegX)
{
	EmitInstruction(0x18, in_RegX, 0, 0);
}

void Emitter::EmitCallImm(const UInt16 in_ImmVal)
{
	EmitInstruction(0x14, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitRet()
{
	EmitInstruction(0x15, 0, 0, 0);
}

void Emitter::EmitCx(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x17, in_RegX, 0, ReverseBytes(in_ImmVal));
}

/////////////// Palette ///////////////
void Emitter::EmitPal(const UInt8 in_RegX)
{
	EmitInstruction(0xD1, in_RegX, 0, 0);
}

void Emitter::EmitPalImm(const UInt16 in_ImmVal)
{
	EmitInstruction(0xD0, 0, 0, ReverseBytes(in_ImmVal));
}

/////////////// Sound ///////////////
void Emitter::EmitSnd0()
{
	EmitInstruction(0x09, 0, 0, 0);
}

void Emitter::EmitSnd1(const UInt16 in_ImmVal)
{
	EmitInstruction(0x0A, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitSnd2(const UInt16 in_ImmVal)
{
	EmitInstruction(0x0B, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitSnd3(const UInt16 in_ImmVal)
{
	EmitInstruction(0x0C, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitSnp(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x0C, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitSng(const UInt8 in_AD, const UInt8 in_SR, const UInt8 in_VT)
{
	// TODO: Maybe this won't work with EmitInstruction...
	// EmitInstruction(0x0E, );
}

/////////////// Misc ///////////////
void Emitter::EmitNop()
{
	EmitInstruction(0, 0, 0, 0);
}

void Emitter::EmitCls()
{
	EmitInstruction(0x01, 0, 0, 0);
}

void Emitter::EmitVBlnk()
{
	EmitInstruction(0x02, 0, 0, 0);
}

void Emitter::EmitBgc(const UInt8 in_Index)
{
	EmitInstruction(0x03, 0, 0, UInt16(in_Index) << 8);
}

void Emitter::EmitSpr(const UInt16 in_ImmVal)
{
	EmitInstruction(0x04, 0, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitDrw(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ)
{
	EmitInstruction(0x06, in_RegX, in_RegY, UInt16(in_RegZ) << 8);
}

void Emitter::EmitDrwImm(const UInt8 in_RegX, const UInt8 in_RegY, const UInt16 in_ImmVal)
{
	EmitInstruction(0x05, in_RegX, in_RegY, ReverseBytes(in_ImmVal));
}

void Emitter::EmitRnd(const UInt8 in_RegX, const UInt16 in_ImmVal)
{
	EmitInstruction(0x07, in_RegX, 0, ReverseBytes(in_ImmVal));
}

void Emitter::EmitFlip(const UInt8 in_X, const UInt8 in_Y)
{
	UInt16 l_Val = in_Y;
	if (in_X)
		l_Val += 2;
	EmitInstruction(0x08, 0, 0, l_Val);
}
