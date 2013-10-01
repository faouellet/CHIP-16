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
	CPUFixture() : Header(16,0) 
	{
		SetupArithmeticData();

		SetupLoadStoreData();
		
		SetupShiftData();
	}

private:
	/*
	* \fn SetupArithmeticData
	* \brief Fills the arithmetic data vectors with opcodes
	*/
	void SetupArithmeticData()
	{
		AddTestData.push_back(0x40);	// ADDI
		AddTestData.push_back(0x00);	// Add 65535 to R0
		AddTestData.push_back(0xFF);
		AddTestData.push_back(0xFF);

		AddTestData.push_back(0x41);	// ADD (inplace)
		AddTestData.push_back(0x10);	// R1 += R0 
		AddTestData.push_back(0x00);
		AddTestData.push_back(0x00);

		AddTestData.push_back(0x42);	// ADD
		AddTestData.push_back(0x03);	// R2 = R0 + R3
		AddTestData.push_back(0x02);
		AddTestData.push_back(0x00);

		AddTestData.push_back(0x42);	// ADD
		AddTestData.push_back(0x04);	// R4 = R0 + R1
		AddTestData.push_back(0x02);	// Should overflow (65535 + 65535)
		AddTestData.push_back(0x00);

		//SubTestData.push_back(0x50);	// SUBI
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//SubTestData.push_back(0x51);	// SUB (inplace)
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//SubTestData.push_back(0x52);	// SUB
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//SubTestData.push_back(0x53);	// CMPI
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//SubTestData.push_back(0x54);	// CMP
		//SubTestData.push_back();
		//SubTestData.push_back();
		//SubTestData.push_back();

		//AndTestData.push_back(0x60);	// ANDI
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//AndTestData.push_back(0x61);	// AND (inplace)
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//AndTestData.push_back(0x62);	// AND
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//AndTestData.push_back(0x63);	// TSTI
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//AndTestData.push_back(0x64);	// TST
		//AndTestData.push_back();
		//AndTestData.push_back();
		//AndTestData.push_back();

		//OrTestData.push_back(0x70);	// ORI
		//OrTestData.push_back();
		//OrTestData.push_back();
		//OrTestData.push_back();

		//OrTestData.push_back(0x71);	// OR (inplace)
		//OrTestData.push_back();
		//OrTestData.push_back();
		//OrTestData.push_back();

		//OrTestData.push_back(0x72);	// OR
		//OrTestData.push_back();
		//OrTestData.push_back();
		//OrTestData.push_back();

		//XorTestData.push_back(0x80);	// XORI
		//XorTestData.push_back();
		//XorTestData.push_back();
		//XorTestData.push_back();

		//XorTestData.push_back(0x81);	// XOR (inplace)
		//XorTestData.push_back();
		//XorTestData.push_back();
		//XorTestData.push_back();

		//XorTestData.push_back(0x82);	// XOR
		//XorTestData.push_back();
		//XorTestData.push_back();
		//XorTestData.push_back();

		//MulTestData.push_back(0x90);	// MULI
		//MulTestData.push_back();
		//MulTestData.push_back();
		//MulTestData.push_back();

		//MulTestData.push_back(0x91);	// MUL (inplace)
		//MulTestData.push_back();
		//MulTestData.push_back();
		//MulTestData.push_back();

		//MulTestData.push_back(0x92);	// MUL
		//MulTestData.push_back();
		//MulTestData.push_back();
		//MulTestData.push_back();
		//
		//DivTestData.push_back(0xA0);	// DIVI
		//DivTestData.push_back();
		//DivTestData.push_back();
		//DivTestData.push_back();

		//DivTestData.push_back(0XA1);	// DIV (inplace)
		//DivTestData.push_back();
		//DivTestData.push_back();
		//DivTestData.push_back();

		//DivTestData.push_back(0XA2);	// DIV
		//DivTestData.push_back();
		//DivTestData.push_back();
		//DivTestData.push_back();
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

	}
};

#endif // CPU_TESTS_H
