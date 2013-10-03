#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "cpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( CPUTestSuite, CPUFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	BOOST_REQUIRE_EQUAL(cpu.Init(PrepareData(AddTestData)), NoError);
	std::vector<Utils::UInt8> l_MemoryDump(cpu.DumpMemory());

	for(unsigned i = 0; i < AddTestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(AddTestData[i], l_MemoryDump[i]);
	for(unsigned i = AddTestData.size(); i < l_MemoryDump.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i], 0);

	BOOST_REQUIRE_EQUAL(cpu.Init(std::vector<UInt8>()), EmptyROMError);
	BOOST_REQUIRE_EQUAL(cpu.Init(std::vector<UInt8>(65535, 42)), ROMOverflowError);
}

BOOST_AUTO_TEST_CASE( AddTest )
{
	cpu.Init(PrepareData(AddTestData));
	cpu.InterpretOp();	// ADDI : R0 += 65535
	cpu.InterpretOp();	// ADD : R1 += R0
	cpu.InterpretOp();	// ADD : R2 = R0 + R3
	cpu.InterpretOp();	// ADD : R3 = R0 + R1	(overflow)
	std::vector<UInt16> l_AddDump(cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_AddDump[0], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[1], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[2], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[3], 65534);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AddDump[i], 0);
}

BOOST_AUTO_TEST_CASE( AndTest )
{
	cpu.Init(PrepareData(AndTestData));
	cpu.InterpretOp();	// ANDI : R0 & 65535
	cpu.InterpretOp();	// AND : R1 &= R0
	cpu.InterpretOp();	// AND : R2 = R0 & R1
	//cpu.InterpretOp();	// TSTI
	//cpu.InterpretOp();	// TST
	std::vector<UInt16> l_AndDump(cpu.DumpRegisters());

	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AndDump[i], 0);
}

BOOST_AUTO_TEST_CASE( DivTest )
{
	cpu.Init(PrepareData(DivTestData));
	cpu.InterpretOp();	// ADDI : R0 += 8
	cpu.InterpretOp();	// ADDI : R1 += 4
	cpu.InterpretOp();	// ADDI : R2 += 2
	cpu.InterpretOp();	// DIVI : R0 /= 2
	cpu.InterpretOp();	// DIV : R1 /= R2
	cpu.InterpretOp();	// DIV : R3 = R2 / R0
	cpu.InterpretOp();	// DIV : R4 = R0 / R2
	std::vector<UInt16> l_DivDump(cpu.DumpRegisters());

	// TODO : Test div by 0
	BOOST_REQUIRE_EQUAL(l_DivDump[0], 4);
	BOOST_REQUIRE_EQUAL(l_DivDump[1], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[2], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[3], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[4], 0);

	for(int i = 5; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_DivDump[i], 0);
}

BOOST_AUTO_TEST_CASE( MulTest )
{
	cpu.Init(PrepareData(MulTestData));
	cpu.InterpretOp();	// ADDI : R0 += 2
	cpu.InterpretOp();	// ADDI : R1 += 2
	cpu.InterpretOp();	// MULI : R0 *= 10
	cpu.InterpretOp();	// MUL : R1 *= R0
	cpu.InterpretOp();	// MUL : R2 = R1 * R0
	cpu.InterpretOp();	// MUL : R3 = R2 * R2 (overflow)
	std::vector<UInt16> l_MulDump(cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_MulDump[0], 20);
	BOOST_REQUIRE_EQUAL(l_MulDump[1], 40);
	BOOST_REQUIRE_EQUAL(l_MulDump[2], 800);
	BOOST_REQUIRE_EQUAL(l_MulDump[3], 50176);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MulDump[i], 0);
}

BOOST_AUTO_TEST_CASE( OrTest )
{
	cpu.Init(PrepareData(OrTestData));
	cpu.InterpretOp();	// ORI : R0 | 1
	cpu.InterpretOp();	// OR : R1 |= R0
	cpu.InterpretOp();	// OR : R2 = R0 | R3
	std::vector<UInt16> l_OrDump(cpu.DumpRegisters());

	for(int i = 0; i < 3; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 1);

	for(int i = 3; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 0);
}

BOOST_AUTO_TEST_CASE( SubTest )
{
	cpu.Init(PrepareData(SubTestData));
	cpu.InterpretOp();	// ADDI : R0 += 12
	cpu.InterpretOp();	// SUBI : R0 -= 4
	cpu.InterpretOp();	// SUB : R1 -= 4
	cpu.InterpretOp();	// SUB : R0 -= R1
	cpu.InterpretOp();	// SUB : R1 -= R0
	cpu.InterpretOp();	// SUB : R2 = R0 - R1
	cpu.InterpretOp();	// SUB : R3 = R1 - R0
	//cpu.InterpretOp();	// CMPI
	//cpu.InterpretOp();	// CMP
	std::vector<UInt16> l_SubDump(cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_SubDump[0], 12);
	BOOST_REQUIRE_EQUAL(l_SubDump[1], 65520);
	BOOST_REQUIRE_EQUAL(l_SubDump[2], 28);
	BOOST_REQUIRE_EQUAL(l_SubDump[3], 65508);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_SubDump[i], 0);
}

BOOST_AUTO_TEST_CASE( XorTest )
{
	cpu.Init(PrepareData(XorTestData));
	cpu.InterpretOp();	// XORI : R0 ^ 1
	cpu.InterpretOp();	// XOR : R1 ^= R0
	cpu.InterpretOp();	// XOR : R2 = R0 ^ R3
	std::vector<UInt16> l_XorDump(cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_XorDump[0], 1);
	BOOST_REQUIRE_EQUAL(l_XorDump[1], 1);

	for(int i = 2; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_XorDump[i], 0);
}

// TODO : Call/Jump tests ??

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	cpu.Init(PrepareData(MemoryTestData));
	cpu.InterpretOp();	// LDI : R0 = 255
	cpu.InterpretOp();	// STM : Memory[R0] = R0
	cpu.InterpretOp();	// LDI : SP = SP + 1
	cpu.InterpretOp();	// STM : Memory[FF] = R0
	cpu.InterpretOp();	// LDM : R2 = Memory[FF]
	cpu.InterpretOp();	// LDM : R3 = Memory[R0]
	cpu.InterpretOp();	// MOV : R2 = R0
	std::vector<UInt16> l_RegisterDump(cpu.DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_RegisterDump[0], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[1], 0);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[2], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[3], 255);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegisterDump[i], 0);

	BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START + 1);

	std::vector<UInt8> l_MemoryDump(cpu.DumpMemory());
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]], l_RegisterDump[0] & 0xFF00);
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]+1], l_RegisterDump[0] & 0x00FF);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
	cpu.Init(PrepareData(StackTestData));
	for(int i = 0; i < NB_REGISTERS; ++i)
		cpu.InterpretOp();	// LDI : Ri = i * 2

	cpu.InterpretOp();	// PUSHALL

	std::vector<Utils::UInt8> l_MemoryDump(cpu.DumpMemory());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i * 2 + STACK_START + 1], i * 2);
	BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START + 32);

	for(int i = 0; i < NB_REGISTERS; ++i)
		cpu.InterpretOp();	// LDI : Ri = 0

	cpu.InterpretOp();	// POPALL

	std::vector<Utils::UInt16> l_RegistersDump(cpu.DumpRegisters());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], i * 2);
	BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START);

	for(int i = 0; i < NB_REGISTERS; ++i)
		cpu.InterpretOp();	// PUSH Ri

	for(int i = 0; i < NB_REGISTERS; ++i)
		cpu.InterpretOp();	// POP Ri

	l_RegistersDump = cpu.DumpRegisters();
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], (NB_REGISTERS - 1 - i) * 2);
	BOOST_REQUIRE_EQUAL(cpu.DumpStackPointer(), STACK_START);
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
	cpu.Init(PrepareData(ShiftTestData));
	for(int i = 0; i < NB_REGISTERS/2; ++i)
		cpu.InterpretOp();	// ADDI : Ri += 65535

	cpu.InterpretOp();	// SHL
	cpu.InterpretOp();	// SHR
	cpu.InterpretOp();	// SAL
	cpu.InterpretOp();	// SAR
	cpu.InterpretOp();	// SHL
	cpu.InterpretOp();	// SHR
	cpu.InterpretOp();	// SAL
	cpu.InterpretOp();	// SAR

	std::vector<UInt16> l_ShiftDump(cpu.DumpRegisters());

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

BOOST_AUTO_TEST_SUITE_END()
