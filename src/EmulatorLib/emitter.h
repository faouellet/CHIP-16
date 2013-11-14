#ifndef EMITTER_H
#define EMITTER_H

#include "memoryarena.h"
#include "utils.h"

using Utils::UInt8;
using Utils::UInt16;

/**
* \class Emitter
* \brief Emit x86 instructions to a memory buffer
* \author Felix-Antoine Ouellet
*/
class Emitter
{
private:
	/**
	* \enum
	* \brief Useful constants
	*/
	enum { DISP32 = 5 };

private:
	Utils::UInt8* m_BufferPtr;	/*!< Pointer to a buffer of executable memory */
	MemoryArena m_Arena;		/*!< Memory arena of executable code */

public:
	/**
	* \fn Emitter
	* \brief Default constructor
	*/
	Emitter();

	/**
	* \fn ~Emitter
	* \brief Destructor
	*/
	~Emitter();

private:
	/**
	* \fn ModRM
	* \brief TODO
	* \param in_Mod
	* \param in_Reg1
	* \param in_Reg2
	*/
	UInt8 ModRM(const UInt8 in_Mod, const UInt8 in_Reg1, const UInt8 in_Reg2) const;

	/**
	* \fn Write8Bits
	* \brief Write a byte to the executable memory buffer
	* \param in_Value Byte to write
	*/
	void Write8Bits(const UInt8 in_Value);
	
	/**
	* \fn Write16Bits
	* \brief Write 2 bytes to the executable memory buffer
	* \param in_Value Concatenated bytes to write
	*/
	void Write16Bits(const UInt16 in_Value);

public:	// Arithmetic instructions
	void ANDIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void ANDREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void ADDIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void ADDREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void CMPIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void CMPREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void DIVIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void DIVREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void MULIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void MULREG(const UInt16 in_Arg1, const UInt16 in_Arg2);
	
	void ORIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void ORREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void SHLIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void SHLREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void SHRIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void SHRREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void SARIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void SARREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void SUBIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void SUBREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void TSTIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);

	void XORIMM(const UInt16 in_Arg1, const UInt16 in_Arg2);
	void XORREG(const UInt16 in_Arg1, const UInt16 in_Arg2);

public:	// Control flow instructions
	void CALL();
	void CX();
	void RET();

public: // Data transfer instructions
	void MOV(const UInt16 in_Source, const UInt16 in_Dest);
	void MOVToStack(const UInt16 in_Source, const UInt16 in_StackOffset);
};

#endif // EMITTER_H
