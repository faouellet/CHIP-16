#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "interpreter_tests.h"

#include "constants.h"

BOOST_FIXTURE_TEST_SUITE( InterpreterTestSuite, InterpreterFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	BOOST_REQUIRE_EQUAL(Cpu->Init(PrepareData(AddTestData)), NoError);
	std::vector<Utils::UInt8> l_MemoryDump(Cpu->DumpMemory());

	for(unsigned i = 0; i < AddTestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(AddTestData[i], l_MemoryDump[i]);
	for(unsigned i = AddTestData.size(); i < l_MemoryDump.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i], 0);

	BOOST_REQUIRE_EQUAL(Cpu->Init(std::vector<UInt8>()), EmptyROMError);
	BOOST_REQUIRE_EQUAL(Cpu->Init(std::vector<UInt8>(65535, 42)), ROMOverflowError);
}

BOOST_AUTO_TEST_CASE( AddTest )
{
	Cpu->Init(PrepareData(AddTestData));
	Interpret.InterpretOp();										// ADDI : R0 += 0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);					// Zero flag set
	Interpret.InterpretOp();										// ADDI : R0 += 65535
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// ADD : R1 += R0
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// ADD : R2 = R0 + R3
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// ADD : R3 = R0 + R1	(overflow)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x40);					// Overflow flag set
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x2);					// Carry flag set
	std::vector<UInt16> l_AddDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_AddDump[0], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[1], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[2], 65535);
	BOOST_REQUIRE_EQUAL(l_AddDump[3], 65534);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AddDump[i], 0);
}

BOOST_AUTO_TEST_CASE( AndTest )
{
	Cpu->Init(PrepareData(AndTestData));
	Interpret.InterpretOp();						// ANDI : R0 & 65535
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);	// Zero flag set
	Interpret.InterpretOp();						// AND : R1 &= R0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);	// Zero flag set
	Interpret.InterpretOp();						// AND : R2 = R0 & R1
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);	// Zero flag set
	//cpu->InterpretOp();	// TSTI
	//cpu->InterpretOp();	// TST
	std::vector<UInt16> l_AndDump(Cpu->DumpRegisters());

	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_AndDump[i], 0);
}

BOOST_AUTO_TEST_CASE( DivTest )
{
	Cpu->Init(PrepareData(DivTestData));
	Interpret.InterpretOp();	// ADDI : R0 += 8
	Interpret.InterpretOp();	// ADDI : R1 += 4
	Interpret.InterpretOp();	// ADDI : R2 += 2
	Interpret.InterpretOp();	// DIVI : R0 /= 32
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);					// Zero flag set
	Interpret.InterpretOp();										// DIV : R1 /= R2
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Interpret.InterpretOp();										// DIV : R3 = R2 / R1
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	std::vector<UInt16> l_DivDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_DivDump[0], 0);
	BOOST_REQUIRE_EQUAL(l_DivDump[1], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[2], 2);
	BOOST_REQUIRE_EQUAL(l_DivDump[3], 1);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_DivDump[i], 0);
}

BOOST_AUTO_TEST_CASE( ErrorTest )
{
	Cpu->Init(PrepareData(ErrorTestData));
	BOOST_REQUIRE_EQUAL(Interpret.InterpretOp(), CPU::StackUnderflow);
	BOOST_REQUIRE_EQUAL(Interpret.InterpretOp(), CPU::MemoryError);
	BOOST_REQUIRE_EQUAL(Interpret.InterpretOp(), CPU::MemoryError);
}

BOOST_AUTO_TEST_CASE( MulTest )
{
	Cpu->Init(PrepareData(MulTestData));
	Interpret.InterpretOp();	// ADDI : R0 += 2
	Interpret.InterpretOp();	// ADDI : R1 += 2
	Interpret.InterpretOp();	// MULI : R0 *= 10
	Interpret.InterpretOp();	// MUL : R1 *= R0
	Interpret.InterpretOp();	// MUL : R2 = R1 * R0
	Interpret.InterpretOp();	// MUL : R3 *= 0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);					// Zero flag set
	Interpret.InterpretOp();										// MUL : R3 = R2 * R2 (overflow)
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x2);					// Carry flag set
	std::vector<UInt16> l_MulDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_MulDump[0], 20);
	BOOST_REQUIRE_EQUAL(l_MulDump[1], 40);
	BOOST_REQUIRE_EQUAL(l_MulDump[2], 800);
	BOOST_REQUIRE_EQUAL(l_MulDump[3], 50176);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MulDump[i], 0);
}

BOOST_AUTO_TEST_CASE( OrTest )
{
	Cpu->Init(PrepareData(OrTestData));
	Interpret.InterpretOp();										// ORI : R0 | 0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);					// Zero flag set
	Interpret.InterpretOp();										// ORI : R0 | 1
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Interpret.InterpretOp();										// OR : R1 |= R0
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Interpret.InterpretOp();										// OR : R2 = R0 | R3
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	std::vector<UInt16> l_OrDump(Cpu->DumpRegisters());

	for(int i = 0; i < 3; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 1);

	for(int i = 3; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_OrDump[i], 0);
}

BOOST_AUTO_TEST_CASE( SubTest )
{
	Cpu->Init(PrepareData(SubTestData));
	Interpret.InterpretOp();	// ADDI : R0 += 12
	Interpret.InterpretOp();	// SUBI : R0 -= 4
	Interpret.InterpretOp();	// SUB : R1 -= 4
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x2);					// Carry flag set
	Interpret.InterpretOp();										// SUB : R0 -= R1
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x2);					// Carry flag set
	Interpret.InterpretOp();										// SUB : R1 -= R0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SUB : R2 = R0 - R1
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	Interpret.InterpretOp();										// SUB : R3 = R1 - R0
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	//cpu->InterpretOp();	// CMPI
	//cpu->InterpretOp();	// CMP
	std::vector<UInt16> l_SubDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_SubDump[0], 12);
	BOOST_REQUIRE_EQUAL(l_SubDump[1], 65520);
	BOOST_REQUIRE_EQUAL(l_SubDump[2], 28);
	BOOST_REQUIRE_EQUAL(l_SubDump[3], 65508);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_SubDump[i], 0);
}

BOOST_AUTO_TEST_CASE( XorTest )
{
	Cpu->Init(PrepareData(XorTestData));
	Interpret.InterpretOp();										// XORI : R0 ^ 1
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Interpret.InterpretOp();										// XOR : R1 ^= R0
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 2) & 0x1, 0);	// Zero flag unset
	Interpret.InterpretOp();										// XOR : R2 = R0 ^ R3
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x4);					// Zero flag set
	std::vector<UInt16> l_XorDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_XorDump[0], 1);
	BOOST_REQUIRE_EQUAL(l_XorDump[1], 1);

	for(int i = 2; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_XorDump[i], 0);
}

// TODO : Call/Jump tests 

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	Cpu->Init(PrepareData(MemoryTestData));
	Interpret.InterpretOp();	// LDI : R0 = 255
	Interpret.InterpretOp();	// STM : Memory[R0] = R0
	Interpret.InterpretOp();	// LDI : SP = SP + 1
	Interpret.InterpretOp();	// STM : Memory[F0] = R0
	Interpret.InterpretOp();	// LDM : R2 = Memory[F0]
	Interpret.InterpretOp();	// LDM : R3 = Memory[R0]
	Interpret.InterpretOp();	// MOV : R2 = R0
	std::vector<UInt16> l_RegisterDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_RegisterDump[0], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[1], 0);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[2], 255);
	BOOST_REQUIRE_EQUAL(l_RegisterDump[3], 255);

	for(int i = 4; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegisterDump[i], 0);

	BOOST_REQUIRE_EQUAL(Cpu->DumpStackPointer(), STACK_START + 1);

	std::vector<UInt8> l_MemoryDump(Cpu->DumpMemory());
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]], l_RegisterDump[0] & 0x00FF);
	BOOST_REQUIRE_EQUAL(l_MemoryDump[l_RegisterDump[0]+1], l_RegisterDump[0] & 0xFF00);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
	Cpu->Init(PrepareData(StackTestData));
	for(int i = 0; i < NB_REGISTERS; ++i)
		Interpret.InterpretOp();	// LDI : Ri = i * 2

	Interpret.InterpretOp();	// PUSHALL

	std::vector<Utils::UInt8> l_MemoryDump(Cpu->DumpMemory());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_MemoryDump[i * 2 + STACK_START], i * 2);
	BOOST_REQUIRE_EQUAL(Cpu->DumpStackPointer(), STACK_START + 32);

	for(int i = 0; i < NB_REGISTERS; ++i)
		Interpret.InterpretOp();	// LDI : Ri = 0

	Interpret.InterpretOp();	// POPALL

	std::vector<Utils::UInt16> l_RegistersDump(Cpu->DumpRegisters());
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], i * 2);
	BOOST_REQUIRE_EQUAL(Cpu->DumpStackPointer(), STACK_START);

	for(int i = 0; i < NB_REGISTERS; ++i)
		Interpret.InterpretOp();	// PUSH Ri

	for(int i = 0; i < NB_REGISTERS; ++i)
		Interpret.InterpretOp();	// POP Ri

	l_RegistersDump = Cpu->DumpRegisters();
	for(int i = 0; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_RegistersDump[i], (NB_REGISTERS - 1 - i) * 2);
	BOOST_REQUIRE_EQUAL(Cpu->DumpStackPointer(), STACK_START);
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
	Cpu->Init(PrepareData(ShiftTestData));
	for(int i = 0; i < NB_REGISTERS/2; ++i)
		Interpret.InterpretOp();	// ADDI : Ri += 65535

	Interpret.InterpretOp();										// SHL : R0 << 13 (Logical)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SHR : R1 >> 13 (Logical)
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	Interpret.InterpretOp();										// SAL : R2 << 13 (Arithmetic)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SAR : R3 >> 13 (Arithmetic)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SHL : R4 << R1 (Logical)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SHR : R5 >> R1 (Logical)
	BOOST_REQUIRE_EQUAL((Cpu->DumpFlagRegister() >> 7) & 0x1, 0);	// Negative flag unset
	Interpret.InterpretOp();										// SAL : R6 << R1 (Arithmetic)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set
	Interpret.InterpretOp();										// SAR : R7 >> R1 (Arithmetic)
	BOOST_REQUIRE(Cpu->DumpFlagRegister() & 0x80);					// Negative flag set

	std::vector<UInt16> l_ShiftDump(Cpu->DumpRegisters());

	BOOST_REQUIRE_EQUAL(l_ShiftDump[0], 57344);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[1], 7);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[2], 57344);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[3], 65535);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[4], 65408);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[5], 511);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[6], 65408);
	BOOST_REQUIRE_EQUAL(l_ShiftDump[7], 65535);
	
	for(int i = NB_REGISTERS/2; i < NB_REGISTERS; ++i)
		BOOST_REQUIRE_EQUAL(l_ShiftDump[i], 0);
}

BOOST_AUTO_TEST_CASE( RndTest )
{
	Cpu->Init(PrepareData(RndTestData));
	for(int i = 0; i < NB_REGISTERS; ++i)
		Interpret.InterpretOp();

	std::vector<UInt16> l_RndDump(Cpu->DumpRegisters());
	for(int i = 0; i < NB_REGISTERS; ++i)
	{
		BOOST_REQUIRE_GE(l_RndDump[i], 0);
		BOOST_REQUIRE_LE(l_RndDump[i], 0xFF00);
	}
}

BOOST_AUTO_TEST_SUITE_END()
