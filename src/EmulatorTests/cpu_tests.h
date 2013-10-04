#ifndef CPU_TESTS_H
#define CPU_TESTS_H

#include "cpu.h"

using Utils::UInt8;

/**
* \struct CPUFixture
* \brief Fixture containing data to test the CPU specific instructions.
*        That means the arithmetic, bitwise, call, jump, load, pop, push,
*        shift and store instructions.
* \author Felix-Antoine Ouellet
*/
struct CPUFixture
{
	CPU Cpu;	/*!< CPU implementation */

	std::vector<Utils::UInt8> Header;	/*!< The header of a .c16 file. See specs for details */

	enum { NB_REGISTERS = 16, STACK_START = 0xFDF0 };

	// Data for the arithmetic tests
	std::vector<UInt8> AddTestData;
	std::vector<UInt8> AndTestData;
	std::vector<UInt8> DivTestData;
	std::vector<UInt8> MulTestData;
	std::vector<UInt8> OrTestData;
	std::vector<UInt8> SubTestData;
	std::vector<UInt8> XorTestData;
	
	// Data for the load/store tests
	std::vector<UInt8> MemoryTestData;

	// Data for the shift (left, arithmetic right, logical right) tests
	std::vector<UInt8> ShiftTestData;

	// Data for the pop/push tests
	std::vector<UInt8> StackTestData;
	
	/**
	* \fn Constructor
	* \brief Setup the data for the cpu instructions tests
	*/
	CPUFixture() : Header(NB_REGISTERS,0) 
	{
		SetupArithmeticData();
		SetupLoadStoreData();
		SetupShiftData();
		SetupStackData();
	}

	/**
	* \fn PrepareData
	* \brief Concatenate a data vector to the header
	* \param in_Data Vector containing the data relevant to a specific instruction
	*                testing procedure
	* \return A data vector
	*/
	std::vector<UInt8> PrepareData(const std::vector<UInt8> & in_Data)
	{
		std::vector<UInt8> l_Data(Header);
		l_Data.insert(l_Data.end(), in_Data.begin(), in_Data.end());
		return l_Data;
	}

	/**
	* \fn PrepareData
	* \brief Insert a 32 bits instruction into a data vector
	* \param out_DataVec Data vector to be inserted into
	* \param in_Op1 Bit[0] to bit[3] of the instruction
	* \param in_Op2 Bit[4] to bit[7] of the instruction
	* \param in_Op3 Bit[8] to bit[11] of the instruction
	* \param in_Op4 Bit[12] to bit[15] of the instruction
	*/
	void InsertInstruction(std::vector<UInt8> & out_DataVec, UInt8 in_Op1, UInt8 in_Op2, UInt8 in_Op3, UInt8 in_Op4)
	{
		out_DataVec.push_back(in_Op1);
		out_DataVec.push_back(in_Op2);
		out_DataVec.push_back(in_Op3);
		out_DataVec.push_back(in_Op4);
	}

private:
	/**
	* \fn SetupArithmeticData
	* \brief Fills the arithmetic data vectors with opcodes
	*/
	void SetupArithmeticData()
	{
		SetupAddTestData();
		SetupAndTestData();
		SetupDivTestData();
		SetupMulTestData();
		SetupOrTestData();
		SetupSubTestData();
		SetupXorTestData();
	}

	/**
	* \fn SetupAddTestData
	* \brief Fills a vector with addition opcodes
	*/
	void SetupAddTestData()
	{
		InsertInstruction(AddTestData, 0x40, 0x00, 0xFF, 0xFF);	// ADDI : R0 += 65535
		InsertInstruction(AddTestData, 0x41, 0x01, 0x00, 0x00);	// ADD : R1 += R0
		InsertInstruction(AddTestData, 0x42, 0x03, 0x02, 0x00);	// ADD : R2 = R0 + R3
		// Should overflow (65535 + 65535)
		InsertInstruction(AddTestData, 0x42, 0x01, 0x03, 0x00);	// ADD : R3 = R0 + R1
	}

	/**
	* \fn SetupAndTestData
	* \brief Fills a vector with and opcodes
	*/
	void SetupAndTestData()
	{
		InsertInstruction(AndTestData, 0x60, 0x00, 0xFF, 0xFF);	// ANDI : R0 & 65535
		InsertInstruction(AndTestData, 0x61, 0x01, 0x00, 0x00);	// AND : R1 &= R0
		InsertInstruction(AndTestData, 0x62, 0x01, 0x02, 0x00);	// AND : R2 = R0 & R1
		
		// TSTI	??
		// TST ??
	}

	/**
	* \fn SetupDivTestData
	* \brief Fills a vector with division opcodes
	*/
	void SetupDivTestData()
	{
		InsertInstruction(DivTestData, 0x40, 0x00, 0x08, 0x00);	// ADDI : R0 += 8
		InsertInstruction(DivTestData, 0x40, 0x01, 0x04, 0x00);	// ADDI : R0 += 4
		InsertInstruction(DivTestData, 0x40, 0x02, 0x02, 0x00);	// ADDI : R0 += 2

		InsertInstruction(DivTestData, 0xA0, 0x00, 0x02, 0x00);	// DIVI : R0 /= 2
		InsertInstruction(DivTestData, 0XA1, 0x21, 0x00, 0x00);	// DIV : R1 /= R2
		InsertInstruction(DivTestData, 0XA2, 0x20, 0x03, 0x00);	// DIV : R3 = R2 / R0
		InsertInstruction(DivTestData, 0XA2, 0x02, 0x04, 0x00);	// DIV : R4 = R0 / R2
	}

	/**
	* \fn SetupLoadStoreData
	* \brief Fills a vector with load/store opcodes
	*/
	void SetupLoadStoreData()
	{
		InsertInstruction(MemoryTestData, 0x20, 0x00, 0xFF, 0x00);	// LDI : R0 = 255
		InsertInstruction(MemoryTestData, 0x31, 0x00, 0x00, 0x00);	// STM : Memory[R0] = R0
		InsertInstruction(MemoryTestData, 0x21, 0x00, 0xF1, 0xFD);	// LDI : SP = SP + 1
		InsertInstruction(MemoryTestData, 0x30, 0x00, 0xFF, 0x00);	// STM : Memory[FF] = R0
		InsertInstruction(MemoryTestData, 0x22, 0x02, 0xFF, 0x00);	// LDM : R2 = Memory[FF]
		InsertInstruction(MemoryTestData, 0x23, 0x03, 0x00, 0x00);	// LDM : R3 = Memory[R0]
		InsertInstruction(MemoryTestData, 0x24, 0x02, 0x00, 0x00);	// MOV : R2 = R0
	}

	/**
	* \fn SetupMulTestData
	* \brief Fills a vector with multiplication opcodes
	*/
	void SetupMulTestData()
	{
		InsertInstruction(MulTestData, 0x40, 0x00, 0x02, 0x00);	// ADDI : R0 += 2
		InsertInstruction(MulTestData, 0x40, 0x01, 0x02, 0x00);	// ADDI : R1 += 2

		InsertInstruction(MulTestData, 0x90, 0x00, 0x0A, 0x00);	// MULI : R0 *= 10
		InsertInstruction(MulTestData, 0x91, 0x01, 0x00, 0x00);	// MUL : R1 *= R0
		InsertInstruction(MulTestData, 0x92, 0x01, 0x02, 0x00);	// MUL : R2 = R1 * R0
		InsertInstruction(MulTestData, 0x92, 0x22, 0x03, 0x00);	// MUL : R3 = R2 * R2
	}

	/**
	* \fn SetupOrTestData
	* \brief Fills a vector with or opcodes
	*/
	void SetupOrTestData()
	{
		InsertInstruction(OrTestData, 0x70, 0x00, 0x01, 0x00);	// ORI : R0 | 1
		InsertInstruction(OrTestData, 0x71, 0x01, 0x00, 0x00);	// OR : R1 |= R0
		InsertInstruction(OrTestData, 0x72, 0x03, 0x02, 0x00);	// OR : R2 = R0 | R3
	}

	/**
	* \fn SetupSubTestData
	* \brief Fills a vector with subtraction opcodes
	*/
	void SetupSubTestData()
	{
		InsertInstruction(SubTestData, 0x40, 0x00, 0x0C, 0x00);	// ADDI : R0 += 12
		
		InsertInstruction(SubTestData, 0x50, 0x00, 0x04, 0x00);	// SUBI : R0 -= 4
		InsertInstruction(SubTestData, 0x50, 0x01, 0x04, 0x00);	// SUB : R1 -= 4
		InsertInstruction(SubTestData, 0x51, 0x10, 0x00, 0x00);	// SUB : R0 -= R1
		InsertInstruction(SubTestData, 0x51, 0x01, 0x00, 0x00);	// SUB : R1 -= R0
		InsertInstruction(SubTestData, 0x52, 0x10, 0x02, 0x00);	// SUB : R2 = R0 - R1
		InsertInstruction(SubTestData, 0x52, 0x01, 0x03, 0x00);	// SUB : R3 = R1 - R0

		/// CMPI ??
		// CMP ??
	}
	
	/**
	* \fn SetupXorTestData
	* \brief Fills a vector with xor opcodes
	*/
	void SetupXorTestData()
	{
		InsertInstruction(XorTestData, 0x80, 0x00, 0x01, 0x00);	// XORI : R0 ^ 1
		InsertInstruction(XorTestData, 0x81, 0x01, 0x00, 0x00);	// XOR : R1 ^= R0
		InsertInstruction(XorTestData, 0x82, 0x01, 0x02, 0x00);	// XOR : R2 = R0 ^ R3
	}

	/**
	* \fn SetupShiftData
	* \brief Fills a vector with shift opcodes
	*/
	void SetupShiftData()
	{
		for(int i = 0; i < NB_REGISTERS/2; ++i)
			InsertInstruction(ShiftTestData, 0x40, i, 0xFF, 0xFF);	// ADDI : Ri += 65535

		InsertInstruction(ShiftTestData, 0xB0, 0x00, 0x0D, 0x00);	// SHL : R0 << 13 (Logical)
		InsertInstruction(ShiftTestData, 0xB1, 0x01, 0x0D, 0x00);	// SHR : R1 >> 13 (Logical)
		InsertInstruction(ShiftTestData, 0xB0, 0x02, 0x0D, 0x00);	// SAL : R2 << 13 (Arithmetic)
		InsertInstruction(ShiftTestData, 0xB2, 0x03, 0x0D, 0x00);	// SAR : R3 >> 13 (Arithmetic)

		InsertInstruction(ShiftTestData, 0xB3, 0x14, 0x00, 0x00);	// SHL : R4 << R1 (Logical)
		InsertInstruction(ShiftTestData, 0xB4, 0x15, 0x00, 0x00);	// SHR : R5 >> R1 (Logical)
		InsertInstruction(ShiftTestData, 0xB3, 0x16, 0x00, 0x00);	// SAL : R6 << R1 (Arithmetic)
		InsertInstruction(ShiftTestData, 0xB5, 0x17, 0x00, 0x00);	// SAR : R7 >> R1 (Arithmetic)
	}

	/**
	* \fn SetupStackData
	* \brief Fills a vector with pop/push opcodes
	*/
	void SetupStackData()
	{
		for(int i = 0; i < NB_REGISTERS; ++i)
			InsertInstruction(StackTestData, 0x20, i, i*2, 0x00);	// LDI : Ri = i * 2

		InsertInstruction(StackTestData, 0xC2, 0x00, 0x00, 0x00);	// PUSHALL

		for(int i = 0; i < NB_REGISTERS; ++i)
			InsertInstruction(StackTestData, 0x20, i, 0x00, 0x00);	// LDI : Ri = 0

		InsertInstruction(StackTestData, 0xC3, 0x00, 0x00, 0x00);	// POPALL

		for(int i = 0; i < NB_REGISTERS; ++i)
			InsertInstruction(StackTestData, 0xC0, i, 0x00, 0x00);	// PUSH Ri

		for(int i = 0; i < NB_REGISTERS; ++i)
			InsertInstruction(StackTestData, 0xC1, i, 0x00, 0x00);	// POP Ri
	}
};

#endif // CPU_TESTS_H
