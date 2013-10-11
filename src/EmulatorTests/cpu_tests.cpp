#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "cpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( CPUTestSuite, CPUFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	BOOST_REQUIRE_EQUAL(Cpu.Init(PrepareData(AddTestData)), NoError);
	std::vector<Utils::UInt8> l_MemoryDump(Cpu.DumpMemory());

	for(unsigned i = 0; i < AddTestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(AddTestData[i], l_MemoryDump[i]);
	for(unsigned i = AddTestData.size(); i < l_MemoryDump.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i], 0);

	BOOST_REQUIRE_EQUAL(Cpu.Init(std::vector<UInt8>()), EmptyROMError);
	BOOST_REQUIRE_EQUAL(Cpu.Init(std::vector<UInt8>(65535, 42)), ROMOverflowError);
}

// TODO : Test negative flag more

BOOST_AUTO_TEST_CASE( AddTest )
{
	Cpu.Init(PrepareData(AddTestData));
	Cpu.InterpretOp();	// ADDI : R0 += 0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
	Cpu.InterpretOp();	// ADDI : R0 += 65535
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// ADD : R1 += R0
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// ADD : R2 = R0 + R3
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// ADD : R3 = R0 + R1	(overflow)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x40);					// Overflow flag set
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
	std::vector<UInt16> l_AddDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_AddDump[0], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[1], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[2], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[3], 65534);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AddDump[i], 0);
}

BOOST_AUTO_TEST_CASE( AndTest )
{
	Cpu.Init(PrepareData(AndTestData));
	Cpu.InterpretOp();	// ANDI : R0 & 65535
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
	Cpu.InterpretOp();	// AND : R1 &= R0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
	Cpu.InterpretOp();	// AND : R2 = R0 & R1
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);	// Zero flag set
	//cpu.InterpretOp();	// TSTI
	//cpu.InterpretOp();	// TST
	std::vector<UInt16> l_AndDump(Cpu.DumpRegisters());

	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AndDump[i], 0);
}

BOOST_AUTO_TEST_CASE( DivTest )
{
	Cpu.Init(PrepareData(DivTestData));
	Cpu.InterpretOp();	// ADDI : R0 += 8
	Cpu.InterpretOp();	// ADDI : R1 += 4
	Cpu.InterpretOp();	// ADDI : R2 += 2
	Cpu.InterpretOp();	// DIVI : R0 = 32 / R0
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Cpu.InterpretOp();	// DIV : R1 = R2 / R1
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
	Cpu.InterpretOp();	// DIV : R3 = R0 / R2
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	std::vector<UInt16> l_DivDump(Cpu.DumpRegisters());

	// TODO : Test div by 0
	BOOST_REQUIRE_EQUAL(l_DivDump[0], 4);
	BOOST_REQUIRE_EQUAL(l_DivDump[1], 0);
	BOOST_REQUIRE_EQUAL(l_DivDump[2], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[3], 2);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_DivDump[i], 0);
}

BOOST_AUTO_TEST_CASE( ErrorTest )
{
	BOOST_REQUIRE(ExecuteBadInstruction(0x0F, 0x00, 0x00, 0x00) & DeviceError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x1F, 0x00, 0x00, 0x00) & CallJumpError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x2F, 0x00, 0x00, 0x00) & MemoryError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x3F, 0x00, 0x00, 0x00) & MemoryError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x4F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x5F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x6F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x7F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x8F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0x9F, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0xAF, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0xBF, 0x00, 0x00, 0x00) & ArithmeticError);
	BOOST_REQUIRE(ExecuteBadInstruction(0xCF, 0x00, 0x00, 0x00) & StackError);
	BOOST_REQUIRE(ExecuteBadInstruction(0xDF, 0x00, 0x00, 0x00) & PaletteError);
	BOOST_REQUIRE(ExecuteBadInstruction(0xEF, 0x00, 0x00, 0x00) & UnknownError);
}

BOOST_AUTO_TEST_CASE( MulTest )
{
	Cpu.Init(PrepareData(MulTestData));
	Cpu.InterpretOp();	// ADDI : R0 += 2
	Cpu.InterpretOp();	// ADDI : R1 += 2
	Cpu.InterpretOp();	// MULI : R0 *= 10
	Cpu.InterpretOp();	// MUL : R1 *= R0
	Cpu.InterpretOp();	// MUL : R2 = R1 * R0
	Cpu.InterpretOp();	// MUL : R3 *= 0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
	Cpu.InterpretOp();	// MUL : R3 = R2 * R2 (overflow)
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
	std::vector<UInt16> l_MulDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_MulDump[0], 20);
	BOOST_REQUIRE_EQUAL(l_MulDump[1], 40);
	BOOST_REQUIRE_EQUAL(l_MulDump[2], 800);
	BOOST_REQUIRE_EQUAL(l_MulDump[3], 50176);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MulDump[i], 0);
}

BOOST_AUTO_TEST_CASE( OrTest )
{
	Cpu.Init(PrepareData(OrTestData));
	Cpu.InterpretOp();	// ORI : R0 | 0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
	Cpu.InterpretOp();	// ORI : R0 | 1
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Cpu.InterpretOp();	// OR : R1 |= R0
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Cpu.InterpretOp();	// OR : R2 = R0 | R3
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	std::vector<UInt16> l_OrDump(Cpu.DumpRegisters());

	for(int i = 0; i < 3; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 1);

	for(int i = 3; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 0);
}

BOOST_AUTO_TEST_CASE( SubTest )
{
	Cpu.Init(PrepareData(SubTestData));
	Cpu.InterpretOp();	// ADDI : R0 += 12
	Cpu.InterpretOp();	// SUBI : R0 -= 4
	Cpu.InterpretOp();	// SUB : R1 -= 4
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x2);					// Carry flag set
	Cpu.InterpretOp();	// SUB : R0 -= R1
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 1) & 0x1, 0);	// Carry flag unset
	Cpu.InterpretOp();	// SUB : R1 -= R0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SUB : R2 = R0 - R1
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	Cpu.InterpretOp();	// SUB : R3 = R1 - R0
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	//cpu.InterpretOp();	// CMPI
	//cpu.InterpretOp();	// CMP
	std::vector<UInt16> l_SubDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_SubDump[0], 12);
	BOOST_REQUIRE_EQUAL(l_SubDump[1], 65520);
	BOOST_REQUIRE_EQUAL(l_SubDump[2], 28);
	BOOST_REQUIRE_EQUAL(l_SubDump[3], 65508);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_SubDump[i], 0);
}

BOOST_AUTO_TEST_CASE( XorTest )
{
	Cpu.Init(PrepareData(XorTestData));
	Cpu.InterpretOp();	// XORI : R0 ^ 1
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Cpu.InterpretOp();	// XOR : R1 ^= R0
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Cpu.InterpretOp();	// XOR : R2 = R0 ^ R3
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x4);					// Zero flag set
	std::vector<UInt16> l_XorDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_XorDump[0], 1);
	BOOST_REQUIRE_EQUAL(l_XorDump[1], 1);

	for(int i = 2; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_XorDump[i], 0);
}

// TODO : Call/Jump tests 

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	Cpu.Init(PrepareData(MemoryTestData));
	Cpu.InterpretOp();	// LDI : R0 = 255
	Cpu.InterpretOp();	// STM : Memory[R0] = R0
	Cpu.InterpretOp();	// LDI : SP = SP + 1
	Cpu.InterpretOp();	// STM : Memory[FF] = R0
	Cpu.InterpretOp();	// LDM : R2 = Memory[FF]
	Cpu.InterpretOp();	// LDM : R3 = Memory[R0]
	Cpu.InterpretOp();	// MOV : R2 = R0
	std::vector<UInt16> l_RegisterDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_RegisterDump[0], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[1], 0);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[2], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[3], 255);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegisterDump[i], 0);

	BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 1);

	std::vector<UInt8> l_MemoryDump(Cpu.DumpMemory());
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]], l_RegisterDump[0] & 0xFF00);
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]+1], l_RegisterDump[0] & 0x00FF);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
	Cpu.Init(PrepareData(StackTestData));
	for(int i = 0; i < NB_REGISTERS; ++i)
		Cpu.InterpretOp();	// LDI : Ri = i * 2

	Cpu.InterpretOp();	// PUSHALL

	std::vector<Utils::UInt8> l_MemoryDump(Cpu.DumpMemory());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i * 2 + STACK_START + 1], i * 2);
	BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START + 32);

	for(int i = 0; i < NB_REGISTERS; ++i)
		Cpu.InterpretOp();	// LDI : Ri = 0

	Cpu.InterpretOp();	// POPALL

	std::vector<Utils::UInt16> l_RegistersDump(Cpu.DumpRegisters());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], i * 2);
	BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START);

	for(int i = 0; i < NB_REGISTERS; ++i)
		Cpu.InterpretOp();	// PUSH Ri

	for(int i = 0; i < NB_REGISTERS; ++i)
		Cpu.InterpretOp();	// POP Ri

	l_RegistersDump = Cpu.DumpRegisters();
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], (NB_REGISTERS - 1 - i) * 2);
	BOOST_REQUIRE_EQUAL(Cpu.DumpStackPointer(), STACK_START);
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
	Cpu.Init(PrepareData(ShiftTestData));
	for(int i = 0; i < NB_REGISTERS/2; ++i)
		Cpu.InterpretOp();	// ADDI : Ri += 65535

	Cpu.InterpretOp();	// SHL : R0 << 13 (Logical)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SHR : R1 >> 13 (Logical)
	BOOST_REQUIRE_EQUAL((Cpu.DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	Cpu.InterpretOp();	// SAL : R2 << 13 (Arithmetic)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SAR : R3 >> 13 (Arithmetic)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SHL : R4 << R1 (Logical)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SHR : R5 >> R1 (Logical)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SAL : R6 << R1 (Arithmetic)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set
	Cpu.InterpretOp();	// SAR : R7 >> R1 (Arithmetic)
	BOOST_REQUIRE(Cpu.DumpFlagRegister() & 0x80);					// Negative flag set

	std::vector<UInt16> l_ShiftDump(Cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_ShiftDump[0], 57344);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[1], 7);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[2], 57344);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[3], 65535);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[4], 65408);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[5], 65535);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[6], 65408);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[7], 65535);
	
	for(int i = NB_REGISTERS/2; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_ShiftDump[i], 0);
}

BOOST_AUTO_TEST_CASE( RndTest )
{
	std::vector<UInt8> l_RndData;
	for(int i = 0; i < NB_REGISTERS; ++i)
		InsertInstruction(l_RndData, 0x07, i, 0x00, 0xFF);	// Ri = Random

	Cpu.Init(PrepareData(l_RndData));
	for(int i = 0; i < NB_REGISTERS; ++i)
		Cpu.InterpretOp();

	std::vector<UInt16> l_RndDump(Cpu.DumpRegisters());
	for(int i = 0; i < NB_REGISTERS; ++i)
	{
		BOOST_REQUIRE_GE(l_RndDump[i], 0);
		BOOST_REQUIRE_LE(l_RndDump[i], 0xFF00);
	}
}

BOOST_AUTO_TEST_SUITE_END()
