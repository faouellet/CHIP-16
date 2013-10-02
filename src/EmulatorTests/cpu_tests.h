#ifndef CPU_TESTS_H
#define CPU_TESTS_H

#include "cpu.h"

/*
* \struct CPUFixture
* \brief Fixture containing data to test the CPU specific instructions.
*        That means the arithmetic, bitwise, call, jump, load, pop, push,
*        shift and store instructions.
*/
struct CPUFixture
{
	CPU cpu;

	std::vector<Utils::UInt8> Header;

	enum { NB_REGISTERS = 16 };

	// Data for the arithmetic tests
	std::vector<Utils::UInt8> AddTestData;
	std::vector<Utils::UInt8> AndTestData;
	std::vector<Utils::UInt8> DivTestData;
	std::vector<Utils::UInt8> MulTestData;
	std::vector<Utils::UInt8> OrTestData;
	std::vector<Utils::UInt8> SubTestData;
	std::vector<Utils::UInt8> XorTestData;
	
	// Data for the load/store tests
	std::vector<Utils::UInt8> MemoryTestData;

	// Data for the shift (left, arithmetic right, logical right) tests
	std::vector<Utils::UInt8> ShiftTestData;
	
	/*
	* \fn Constructor
	* \brief Setup the data for the cpu instructions tests
	*/
	CPUFixture() : Header(NB_REGISTERS,0) 
	{
		SetupArithmeticData();
		SetupLoadStoreData();
		SetupShiftData();
	}

	/*
	* \fn PrepareData
	* \brief Concatenate a data vector to the header
	* \param in_Data Vector containing the data relevant to a specific instruction
	*                testing procedure
	* \return A data vector
	*/
	std::vector<Utils::UInt8> PrepareData(const std::vector<Utils::UInt8> & in_Data)
	{
		std::vector<Utils::UInt8> l_Data(Header);
		l_Data.insert(l_Data.end(), in_Data.begin(), in_Data.end());
		return l_Data;
	}

private:
	/*
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

	/*
	* \fn SetupAddTestData
	* \brief Fills a vector with addition opcodes
	*/
	void SetupAddTestData()
	{
		AddTestData.push_back(0x40);	// ADDI
		AddTestData.push_back(0x00);	// R0 += 65535
		AddTestData.push_back(0xFF);
		AddTestData.push_back(0xFF);

		AddTestData.push_back(0x41);	// ADD (inplace)
		AddTestData.push_back(0x01);	// R1 += R0 
		AddTestData.push_back(0x00);
		AddTestData.push_back(0x00);

		AddTestData.push_back(0x42);	// ADD
		AddTestData.push_back(0x03);	// R2 = R0 + R3
		AddTestData.push_back(0x02);
		AddTestData.push_back(0x00);

		AddTestData.push_back(0x42);	// ADD
		AddTestData.push_back(0x01);	// R4 = R0 + R1
		AddTestData.push_back(0x03);	// Should overflow (65535 + 65535)
		AddTestData.push_back(0x00);
	}

	/*
	* \fn SetupAndTestData
	* \brief Fills a vector with and opcodes
	*/
	void SetupAndTestData()
	{
		AndTestData.push_back(0x60);	// ANDI
		AndTestData.push_back(0x00);
		AndTestData.push_back(0xFF);
		AndTestData.push_back(0xFF);

		AndTestData.push_back(0x61);	// AND (inplace)
		AndTestData.push_back(0x01);
		AndTestData.push_back(0x00);
		AndTestData.push_back(0x00);

		AndTestData.push_back(0x62);	// AND
		AndTestData.push_back(0x01);
		AndTestData.push_back(0x02);
		AndTestData.push_back(0x00);

		//AndTestData.push_back(0x63);	// TSTI
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//AndTestData.push_back(0x64);	// TST
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();
	}

	/*
	* \fn SetupDivTestData
	* \brief Fills a vector with division opcodes
	*/
	void SetupDivTestData()
	{
		// R0 += 8
		DivTestData.push_back(0x40);
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x08);

		// R1 += 4
		DivTestData.push_back(0x40);
		DivTestData.push_back(0x01);
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x04);

		// R2 += 2
		DivTestData.push_back(0x40);
		DivTestData.push_back(0x02);
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x02);

		DivTestData.push_back(0xA0);	// DIVI
		DivTestData.push_back(0x00);	// R0 /= 2
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x02);

		DivTestData.push_back(0XA1);	// DIV (inplace)
		DivTestData.push_back(0x21);	// R1 /= R2
		DivTestData.push_back(0x00);
		DivTestData.push_back(0x00);

		DivTestData.push_back(0XA2);	// DIV
		DivTestData.push_back(0x20);	// R3 = R2 / R0
		DivTestData.push_back(0x03);
		DivTestData.push_back(0x00);

		DivTestData.push_back(0XA2);	// DIV
		DivTestData.push_back(0x02);	// R4 = R0 / R2
		DivTestData.push_back(0x04);
		DivTestData.push_back(0x00);
	}

	/*
	* \fn SetupMulTestData
	* \brief Fills a vector with multiplication opcodes
	*/
	void SetupMulTestData()
	{
		// R0 += 2
		MulTestData.push_back(0x40);
		MulTestData.push_back(0x00);
		MulTestData.push_back(0x00);
		MulTestData.push_back(0x02);

		// R1 += 2
		MulTestData.push_back(0x40);
		MulTestData.push_back(0x01);
		MulTestData.push_back(0x00);
		MulTestData.push_back(0x02);

		MulTestData.push_back(0x90);	// MULI
		MulTestData.push_back(0x00);	// R0 *= 10
		MulTestData.push_back(0x00);
		MulTestData.push_back(0x0A);

		MulTestData.push_back(0x91);	// MUL (inplace)
		MulTestData.push_back(0x01);	// R1 *= R0
		MulTestData.push_back(0x00);
		MulTestData.push_back(0x00);

		MulTestData.push_back(0x92);	// MUL
		MulTestData.push_back(0x01);	// R2 = R1 * R0
		MulTestData.push_back(0x02);
		MulTestData.push_back(0x00);
	}

	/*
	* \fn SetupOrTestData
	* \brief Fills a vector with or opcodes
	*/
	void SetupOrTestData()
	{
		OrTestData.push_back(0x70);	// ORI
		OrTestData.push_back(0x00);	// R0 | 1
		OrTestData.push_back(0x00);
		OrTestData.push_back(0x01);

		OrTestData.push_back(0x71);	// OR (inplace)
		OrTestData.push_back(0x01);	// R1 |= R0
		OrTestData.push_back(0x00);
		OrTestData.push_back(0x00);

		OrTestData.push_back(0x72);	// OR
		OrTestData.push_back(0x03);	// R2 = R0 | R3
		OrTestData.push_back(0x02);
		OrTestData.push_back(0x00);
	}

	/*
	* \fn SetupSubTestData
	* \brief Fills a vector with subtraction opcodes
	*/
	void SetupSubTestData()
	{
		// R0 += 12
		SubTestData.push_back(0x40);
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x0C);

		SubTestData.push_back(0x50);	// SUBI
		SubTestData.push_back(0x00);	// R0 -= 4
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x04);

		SubTestData.push_back(0x50);	// SUBI
		SubTestData.push_back(0x01);	// R1 -= 4
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x04);

		SubTestData.push_back(0x51);	// SUB (inplace)
		SubTestData.push_back(0x10);	// R0 -= R1  
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x00);

		SubTestData.push_back(0x51);	// SUB (inplace)
		SubTestData.push_back(0x01);	// R1 -= R0
		SubTestData.push_back(0x00);
		SubTestData.push_back(0x00);

		SubTestData.push_back(0x52);	// SUB
		SubTestData.push_back(0x10);	// R2 = R0 - R1
		SubTestData.push_back(0x02);
		SubTestData.push_back(0x00);

		SubTestData.push_back(0x52);	// SUB
		SubTestData.push_back(0x01);	// R3 = R1 - R0
		SubTestData.push_back(0x03);
		SubTestData.push_back(0x00);

		//SubTestData.push_back(0x53);	// CMPI
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//SubTestData.push_back(0x54);	// CMP
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();
	}
	
	/*
	* \fn SetupXorTestData
	* \brief Fills a vector with xor opcodes
	*/
	void SetupXorTestData()
	{
		XorTestData.push_back(0x80);	// XORI
		XorTestData.push_back(0x00);	// R0 ^ 1
		XorTestData.push_back(0x00);
		XorTestData.push_back(0x01);

		XorTestData.push_back(0x81);	// XOR (inplace)
		XorTestData.push_back(0x01);	// R1 ^= R0
		XorTestData.push_back(0x00);
		XorTestData.push_back(0x00);

		XorTestData.push_back(0x82);	// XOR
		XorTestData.push_back(0x01);	// R2 = R0 ^ R1
		XorTestData.push_back(0x02);
		XorTestData.push_back(0x00);
	}

	/*
	* \fn SetupLoadStoreData
	* \brief Fills a vector with load/store opcodes
	*/
	void SetupLoadStoreData()
	{
		
	}

	/*
	* \fn SetupShiftData
	* \brief Fills a vector with shift opcodes
	*/
	void SetupShiftData()
	{
		// R0 += 8
		ShiftTestData.push_back(0x40);
		ShiftTestData.push_back(0x00);
		ShiftTestData.push_back(0x00);
		ShiftTestData.push_back(0x08);

		// R1 += 4
		ShiftTestData.push_back(0x40);
		ShiftTestData.push_back(0x01);
		ShiftTestData.push_back(0x00);
		ShiftTestData.push_back(0x04);

		// R2 += 2
		ShiftTestData.push_back(0x40);
		ShiftTestData.push_back(0x02);
		ShiftTestData.push_back(0x00);
		ShiftTestData.push_back(0x02);
	}
};

#endif // CPU_TESTS_H
