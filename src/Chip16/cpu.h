#ifndef CPU_H
#define CPU_H

#include <vector>

#include "gpu.h"
#include "spu.h"

using namespace Utils;

// For tag dispatching when emulating arithmetic instructions
struct Tag { };
struct AddTag : Tag { };
struct DivTag : Tag { };
struct MulTag : Tag { };
struct SubTag : Tag { };

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
	enum { HEADER_SIZE = 16, MEMORY_SIZE = 64*1024 };

private:
	UInt16 m_PC;			/*!< Program counter */
	UInt16 m_SP;			/*!< Stack pointer */
	UInt8 m_FR;				/*!< Flag register */
	UInt16 m_Registers[16];	/*!< General purpose registers */

	UInt8 m_ROMHeader[HEADER_SIZE];	/*!< The header of a .c16 file. See specs for details*/
	UInt8 m_Memory[MEMORY_SIZE];	/*!< Memory of the CPU. See specs for layout details */

	GPU m_GPU;				/*!< The graphics processing unit */
	SPU m_SPU;				/*!< The sound processing unit */

private:
	/**
	* \enum
	* \brief Masks for the flag register
	*/
	enum { UnsignedCarryFlag = 2, UnsignedBorrowFlag = 2, ZeroFlag = 4, 
			SignedOverflowFlag = 64, NegativeFlag = 128 };

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
	* \return Error code
	*/
	unsigned Init(std::vector<UInt8> && in_ROMData);

	/**
	* \fn InterpretOp
	* \n brief Read an opcode from the ROM and execute it
	*/
	void InterpretOp();

private:
	/*
	* \fn FetchImmediateValue
	* \brief Concatenate the value pointed by the PC with the one the PC is going
	*        to point next. Note that this function will increment the PC by 2.
	* \return The immediate value at the end of the opcode
	*/
	UInt16 FetchImmediateValue();

	/*
	* \fn FetchRegisterAddress
	* \brief Extract the address given at where the PC is pointed.
	*        This function increment the PC by 1.
	* \return A register address
	*/
	UInt16 FetchRegisterAddress();

	/*
	* \fn FetchImmediateValue
	* \brief Extract the values contained within the registers whose addresses
	*        are in the byte pointed by the PC
	*/
	void FetchRegistersValues(UInt16 & out_X, UInt16 & out_Y);

	/*
	* \fn Pop
	* \brief Pop a value from the stack of the emulator and decrement the SP by 2
	* \return The value that was at the top of the stack
	*/
	UInt16 Pop();

	/*
	* \fn Push
	* \brief Push a value on the stack of the emulator and increment the SP by 2
	* \param in_Val The value to be pushed
	*/
	void Push(UInt16 in_Val);

	/**
	* \fn SetSignZeroFlag
	* \brief Set the sign flag if the bit[15] of the result is lit
	*        and set the zero flag if the result is zero
	*/
	void SetSignZeroFlag(UInt16 in_Result);

	/**
	* \fn SetCarryOverflowFlag
	* \brief Set the sign flag if the bit[15] of the result is lit and set
	*        the zero flag if the result is zero. Tag dispatching is used to
	*        use the correct behavior for a given instruction type
	* \param in_Op1 The left hand side operand in a computation
	* \param in_Op2 The right hand side operand in a computation
	* \param Tag Indication regarding the instruction type
	*/

	void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, AddTag);
	void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, DivTag);
	void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, MulTag);
	void SetCarryOverflowFlag(UInt16 in_Op1, UInt16 in_Op2, SubTag);

private:
	/**
	* \fn InterpretArithmetics
	* \brief Decode and execute an arithmetic opcode
	* \tparam A functor type
	* \param The function to apply to the operand pointed by the PC
	*/
	template<class F>
	void InterpretArithmetics(F in_Func, Tag in_Tag)	// TODO : Check the tag dispatching works correctly
	{
		switch (m_Memory[m_PC++] & 0xF)
		{
			case 0x0:	// X = F(X, I)
			{
				UInt16 l_Addr = FetchRegistersAddress();
				UInt16 l_IVal = FetchImmediateValue();
				SetCarryOverflowFlag(l_Addr, l_IVal, in_Tag);
				m_Registers[l_Addr] = in_Func(m_Registers[l_Addr], l_IVal);
				SetNegativeZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
				break;
			}
			case 0x1:	// X = F(X, Y)
			{
				UInt16 l_XVal, l_YVal;
				FetchRegistersValues(l_XVal, l_YVal);
				SetCarryOverflowFlag(l_XVal, l_YVal, in_Tag);
				m_Registers[m_Memory[m_PC] & 0xF] = in_Func(l_XVal, l_YVal);
				SetNegativeZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
				m_PC += 3;
				break;
			}
			case 0x2:	// Z = F(X, Y)
			{
				UInt16 l_XVal, l_YVal;
				FetchRegistersValues(l_XVal, l_YVal);
				SetCarryOverflowFlag(l_XVal, l_YVal, in_Tag);
				m_Registers[m_Memory[++m_PC] & 0xF] = in_Func(l_XVal, l_YVal);
				SetNegativeZeroFlag(m_Registers[m_Memory[m_PC] & 0xF]);
				m_PC += 2;
				break;
			}
			case 0x3:	// F(X, I)
			{
				UInt16 l_Addr = FetchRegistersAddress();
				UInt16 l_IVal = FetchImmediateValue();
				UInt16 l_Result = in_Func(m_Registers[l_Addr], l_IVal);
				SetNegativeZeroFlag(l_Result);
				break;
			}
			case 0x4:	// F(X, Y)
			{
				UInt16 l_XVal, l_YVal;
				FetchRegistersValues(l_XVal, l_YVal);
				UInt16 l_Result = in_Func(l_XVal, l_YVal);
				SetNegativeZeroFlag(l_Result);
				m_PC += 3;
				break;
			}
			default:
			{
				// PANIC !!!
				// TODO : Panic behavior
				break;
			}
		}
	}

	/**
	* \fn InterpretCallJumps
	* \brief Decode and execute a call or jump opcode
	*/
	void InterpretCallJumps();

	/**
	* \fn InterpretConditions
	* \brief Decode and execute a condition
	* \param in_CondCode A byte containing the condition to be evaluated
	* \return Condition evalution result
	*/
	bool InterpretConditions(UInt8 in_CondCode);
	
	/**
	* \fn InterpretLoads
	* \brief Decode and execute a load opcode
	*/
	void InterpretLoads();

	// TODO : Find a better name
	/**
	* \fn InterpretMisc
	* \brief Decode and execute a misc opcode
	*/
	void InterpretMisc();

	/**
	* \fn InterpretPalettes
	* \brief Decode and execute a palette opcode
	*/
	void InterpretPalettes();

	/**
	* \fn InterpretPushPops
	* \brief Decode and execute a push or pop opcode
	*/
	void InterpretPushPops();

	/**
	* \fn InterpretShifts
	* \brief Decode and execute a shift opcode
	*/
	void InterpretShifts();

	/**
	* \fn InterpretStores
	* \brief Decode and execute a store  opcode
	*/
	void InterpretStores();
};

#endif // CPU_H
