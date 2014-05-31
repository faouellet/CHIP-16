#ifndef EMITTER_TESTS_H
#define EMITTER_TESTS_H

#include "emitter.h"

#include <fstream>

/**
* \struct EmitterFixture
* \brief Fixture containing data to test the emitter functions.
* \author Felix-Antoine Ouellet
*/
struct EmitterFixture
{
	const char const* TestFilename = "test.c16";

	Emitter Emitter;

	std::vector<UInt8> ArithmeticTestData;
	std::vector<UInt8> StackTestData;
	std::vector<UInt8> MemoryTestData;
	std::vector<UInt8> JumpTestData;
	std::vector<UInt8> PaletteTestData;
	std::vector<UInt8> MiscTestData;
	std::vector<UInt8> SoundTestData;

	EmitterFixture() 
	{
		SetupArithmeticTest();
		SetupStackTest();
		SetupMemoryTest();
		SetupJumpTest();
		SetupPaletteTest();
		SetupMiscTest();
		SetupSoundTest();
	}

	std::vector<UInt8> ReadFile()
	{
		std::ifstream l_TestFileStream(TestFilename);
		
		if (l_TestFileStream.is_open())
		{
			std::vector<UInt8> l_TestData;
			l_TestFileStream.seekg(0, std::ios::end);
			auto size = l_TestFileStream.tellg();
			l_TestFileStream.seekg(0, std::ios::beg);

			l_TestData.resize(size);
			l_TestFileStream.read((char*)&l_TestData[0], size);

			l_TestFileStream.close();

			// Don't care about the header
			return std::vector<UInt8>(l_TestData.begin()+16, l_TestData.end());
		}

		return std::vector<UInt8>();
	}

private:
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

	void SetupArithmeticTest()
	{
		InsertInstruction(ArithmeticTestData, 0x42, 0x21, 0x03, 0x00); // ADD: R3 = R1 + R2
		InsertInstruction(ArithmeticTestData, 0x40, 0x01, 0xFF, 0x00); // ADDI: R1 += 0x00FF
		InsertInstruction(ArithmeticTestData, 0x41, 0x21, 0x00, 0x00); // ADD: R1 += R2
		
		InsertInstruction(ArithmeticTestData, 0x62, 0x45, 0x06, 0x00); // AND: R6 = R4 & R5
		InsertInstruction(ArithmeticTestData, 0x60, 0x01, 0x00, 0xAA); // ANDI: R1 &= 0xAA00
		InsertInstruction(ArithmeticTestData, 0x61, 0x23, 0x00, 0x00); // AND: R3 &= R2
		
		InsertInstruction(ArithmeticTestData, 0xB5, 0x21, 0x00, 0x00); // SAR: R1 >>= R2
		InsertInstruction(ArithmeticTestData, 0xB2, 0x03, 0x02, 0x00); // SAR: R3 >>= 2

		InsertInstruction(ArithmeticTestData, 0x54, 0x65, 0x00, 0x00); // CMP: R6 - R5
		InsertInstruction(ArithmeticTestData, 0x53, 0x4, 0x00, 0xFF); // CMPI: R4 - 0xFF00

		InsertInstruction(ArithmeticTestData, 0xA2, 0x21, 0x03, 0x00); // DIV: R3 = R1 / R2
		InsertInstruction(ArithmeticTestData, 0xA0, 0x01, 0xFF, 0x00); // DIVI: R1 /= 0x00FF
		InsertInstruction(ArithmeticTestData, 0xA1, 0x21, 0x00, 0x00); // DIV: R1 /= R2

		InsertInstruction(ArithmeticTestData, 0xB3, 0x21, 0x00, 0x00); // SHL: R1 <<= R2
		InsertInstruction(ArithmeticTestData, 0xB0, 0x01, 0x02, 0x00); // SHL: R1 <<= 2

		InsertInstruction(ArithmeticTestData, 0xB4, 0x21, 0x00, 0x00); // SHR: R1 >>= R2
		InsertInstruction(ArithmeticTestData, 0xB1, 0x02, 0x04, 0x00); // SHR: R2 >>= 4

		InsertInstruction(ArithmeticTestData, 0x92, 0x21, 0x03, 0x00); // MUL: R3 = R1 * R2	
		InsertInstruction(ArithmeticTestData, 0x90, 0x01, 0xFF, 0x00); // MULI: R1 *= 0x00FF
		InsertInstruction(ArithmeticTestData, 0x91, 0x21, 0x00, 0x00); // MUL: R1 *= R2
		
		InsertInstruction(ArithmeticTestData, 0x72, 0x21, 0x03, 0x00); // OR: R3 = R1 | R2
		InsertInstruction(ArithmeticTestData, 0x70, 0x01, 0xFF, 0x00); // ORI: R1 |= 0x00FF
		InsertInstruction(ArithmeticTestData, 0x71, 0x23, 0x00, 0x00); // OR: R1 |= R2
		
		InsertInstruction(ArithmeticTestData, 0x52, 0x21, 0x03, 0x00); // SUB: R3 = R2 - R1
		InsertInstruction(ArithmeticTestData, 0x50, 0x01, 0xEE, 0x00); // SUBI: R1 -= 0x00EE
		InsertInstruction(ArithmeticTestData, 0x51, 0x21, 0x00, 0x00); // SUB: R1 -=R2
		
		InsertInstruction(ArithmeticTestData, 0x64, 0x23, 0x00, 0x00); // TST: R2 & R3		
		InsertInstruction(ArithmeticTestData, 0x63, 0x01, 0xFF, 0xFF); // TSTI: R1 & 0xFFFF
		
		InsertInstruction(ArithmeticTestData, 0x82, 0x21, 0x03, 0x00); // XOR: R3 = R1 ^ R2	
		InsertInstruction(ArithmeticTestData, 0x80, 0x01, 0x00, 0xFF); // XORI: R1 ^ 0xFF00
		InsertInstruction(ArithmeticTestData, 0x81, 0x21, 0x00, 0x00); // XOR: R1 ^= R2		
	}

	void SetupStackTest()
	{
		InsertInstruction(StackTestData, 0xC0, 0x01, 0x00, 0x00); // PUSH R1
		InsertInstruction(StackTestData, 0xC2, 0x02, 0x00, 0x00); // POP R2
		InsertInstruction(StackTestData, 0xC3, 0x00, 0x00, 0x00); // PUSHALL
		InsertInstruction(StackTestData, 0xC3, 0x00, 0x00, 0x00); // POPALL
		InsertInstruction(StackTestData, 0xC4, 0x00, 0x00, 0x00); // PUSHF
		InsertInstruction(StackTestData, 0xC5, 0x00, 0x00, 0x00); // POPF
	}

	void SetupMemoryTest()
	{
		InsertInstruction(MemoryTestData, 0x20, 0x01, 0xFF, 0x00); // LDI R1 0x00FF
		InsertInstruction(MemoryTestData, 0x21, 0x00, 0xFF, 0x00); // LDI SP 0x00FF

		
		InsertInstruction(MemoryTestData, 0x23, 0x21, 0x00, 0x00); // LDM R1 R2
		InsertInstruction(MemoryTestData, 0x22, 0x01, 0xFF, 0x00); // LDM R1 0x00FF
		
		InsertInstruction(MemoryTestData, 0x24, 0x21, 0x00, 0x00); // MOV R1 R2

		InsertInstruction(MemoryTestData, 0x31, 0x21, 0x00, 0x00); // STM R1 R2
		InsertInstruction(MemoryTestData, 0x30, 0x01, 0xFF, 0x00); // STM R1 0x00FF
	}

	void SetupJumpTest()
	{
		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );

		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );
		InsertInstruction(JumpTestData, );
	}

	void SetupPaletteTest()
	{
		InsertInstruction(PaletteTestData, );
		InsertInstruction(PaletteTestData, );
	}

	void SetupMiscTest()
	{
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
		InsertInstruction(MiscTestData, );
	}

	void SetupSoundTest()
	{
		InsertInstruction(SoundTestData, );
		InsertInstruction(SoundTestData, );
		InsertInstruction(SoundTestData, );
		InsertInstruction(SoundTestData, );
		InsertInstruction(SoundTestData, );
		InsertInstruction(SoundTestData, );
	}
};

#endif // EMITTER_TESTS_H
