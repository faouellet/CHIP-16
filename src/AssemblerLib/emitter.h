#ifndef ASM_EMITTER_H
#define ASM_EMITTER_H

#include <fstream>
#include <string>
#include <vector>

#include "../Utils/utils.h"

using Utils::UInt8;
using Utils::UInt16;
using Utils::UInt32;

/*
 * \class Emitter
 * \brief Produce a binary file encoded with Chip16 instructions
 */
class Emitter
{
private:
	UInt32 m_MagicNumber;
	UInt8 m_VersionNumer;
	UInt8 m_Zero;
    std::vector<UInt32> m_Buffer;

public:
    /*
     * \fn Emitter
     * \brief Default constructor
     * \param in_BufSize Size of the buffer to generate in bytes
     */
    Emitter(const UInt32 & in_BufSize = 0);

    /*
     * \fn ~Emitter
     * \brief Destructor
     */
    ~Emitter();

	/*
	* \fn EmitInstruction
	* \brief Dump the Emitter instructions buffer in a file
	* \param in_Filename Name of the file to writethe Chip16 instructions to
	*/
    void EmitToFile(const std::string & in_Filename);

private:
	/*
	* \fn EmitInstruction
	* \brief Emits a Chip16 instruction in the Emitter internal buffer
	* \param in_Opcode Opcode of the instruction
	* \param in_Op1 First operand of the instruction
	* \param in_Op1 Second operand of the instruction
	* \param in_Op1 Third operand of the instruction (can be 16 bits immediate value)
	*/
    void EmitInstruction(const UInt8 in_Opcode, const UInt8 in_Op1, 
            const UInt8 in_Op2, const UInt16 in_Op3);
	/*
	* \fn InverseBytes
	* \brief Invert the two bytes making up an UInt16
	* \param in_Bytes The two bytes to invert
	* \return The inverted bytes
	*/
	UInt16 ReverseBytes(const UInt16 in_Bytes);

	/*
	* \fn WriteHeader
	* \brief Write the Chip16 file header (See spec for details)
	* \param io_FStream Stream to the file to write to
	*/
	void WriteHeader(std::ofstream & io_FStream);

	/*
	* \fn WriteLittleEndian
	* \brief Invert the two bytes making up an UInt16
	* \param io_FStream Stream to the file to write to
	* \param in_Bytes The four bytes to write
	*/
	void WriteLittleEndian(std::ofstream & io_FStream, const UInt32 in_Bytes);

public:    /////////////// Arithmetric ///////////////
    void EmitAdd(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
    void EmitAddImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
    void EmitAddInp(const UInt8 in_RegX, const UInt8 in_RegY);

	void EmitAnd(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitAndImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitAndInp(const UInt8 in_RegX, const UInt8 in_RegY);

    void EmitARShift(const UInt8 in_RegX, const UInt8 in_RegY);
	void EmitARShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal);
    
	void EmitCmp(const UInt8 in_RegX, const UInt8 in_RegY);
	void EmitCmpImm(const UInt8 in_RegX, const UInt16 in_ImmVal);

	void EmitDiv(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitDivImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitDivInp(const UInt8 in_RegX, const UInt8 in_RegY);

    void EmitLLShift(const UInt8 in_RegX, const UInt8 in_RegY);
    void EmitLLShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal);

    void EmitLRShift(const UInt8 in_RegX, const UInt8 in_RegY);
    void EmitLRShiftImm(const UInt8 in_RegX, const UInt8 in_ImmVal);
    
	void EmitMul(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitMulImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitMulInp(const UInt8 in_RegX, const UInt8 in_RegY);

	void EmitOr(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitOrImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitOrInp(const UInt8 in_RegX, const UInt8 in_RegY);

	void EmitSub(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitSubImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitSubInp(const UInt8 in_RegX, const UInt8 in_RegY);

	void EmitTst(const UInt8 in_RegX, const UInt8 in_RegY);
    void EmitTstImm(const UInt8 in_Op1, const UInt16 in_ImmVal);

	void EmitXor(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitXorImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitXorInp(const UInt8 in_RegX, const UInt8 in_RegY);

public:    /////////////// Stack ///////////////
    void EmitPush(const UInt8 in_RegID);
    void EmitPop(const UInt8 in_RegID);
    void EmitPushAll();
    void EmitPopAll();
    void EmitPushF();
    void EmitPopF();

public:    /////////////// Memory ///////////////
	void EmitLDIImm(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitLDISP(const UInt16 in_ImmVal);

	void EmitLDM(const UInt8 in_RegX, const UInt8 in_RegY);
	void EmitLDMImm(const UInt8 in_RegX, const UInt16 in_ImmVal);

	void EmitMov(const UInt8 in_RegX, const UInt8 in_RegY);

	void EmitStm(const UInt8 in_RegX, const UInt8 in_RegY);
	void EmitStmImm(const UInt8 in_RegX, const UInt16 in_ImmVal);

public:    /////////////// Jump ///////////////
	void EmitJmp(const UInt16 in_ImmVal);
	void EmitJx(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitJme(const UInt8 in_RegX, const UInt8 in_RegY, const UInt16 in_ImmVal);
	void EmitJmpInd(const UInt8 in_RegX);

	void EmitCall(const UInt8 in_RegX);
	void EmitCallImm(const UInt16 in_ImmVal);
	void EmitRet();
	void EmitCx(const UInt8 in_RegX, const UInt16 in_ImmVal);

public:    /////////////// Palette ///////////////
	void EmitPal(const UInt8 in_RegX);
	void EmitPalImm(const UInt16 in_ImmVal);

public:    /////////////// Misc ///////////////
	void EmitNop();
	void EmitCls();
	void EmitVBlnk();
	void EmitBgc(const UInt8 in_Index);
	void EmitSpr(const UInt16 in_ImmVal);
	void EmitDrw(const UInt8 in_RegX, const UInt8 in_RegY, const UInt8 in_RegZ);
	void EmitDrwImm(const UInt8 in_RegX, const UInt8 in_RegY, const UInt16 in_ImmVal);
	void EmitRnd(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitFlip(const UInt8 in_X, const UInt8 in_Y);

public:    /////////////// Sound ///////////////
	void EmitSnd0();
	void EmitSnd1(const UInt16 in_ImmVal);
	void EmitSnd2(const UInt16 in_ImmVal);
	void EmitSnd3(const UInt16 in_ImmVal);
	void EmitSnp(const UInt8 in_RegX, const UInt16 in_ImmVal);
	void EmitSng(const UInt8 in_AD, const UInt8 in_SR, const UInt8 in_VT);
};

#endif // ASM_EMITTER_H

