#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "cpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( CPUTestSuite, CPUFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	std::vector<Utils::UInt8> l_Data(Header);
	l_Data.insert(l_Data.begin(), AddTestData.begin(), AddTestData.end());
	cpu.Init(std::move(l_Data));
	std::vector<Utils::UInt8> l_MemoryDump(cpu.DumpMemory());

	for(unsigned i = 0; i < l_MemoryDump.size(); ++i)
		BOOST_REQUIRE(AddTestData[i] == l_MemoryDump[i]);
}

BOOST_AUTO_TEST_CASE( AddTest )
{
	cpu.Init(std::move(AddTestData));
	cpu.InterpretOp();	// ADDI
	cpu.InterpretOp();	// ADD	(inplace)
	cpu.InterpretOp();	// ADD
	cpu.InterpretOp();	// ADD	(overflow)
	std::vector<UInt16> l_AddDump(cpu.DumpRegisters());

	BOOST_REQUIRE(l_AddDump[0] == 15);
	BOOST_REQUIRE(l_AddDump[1] == 15);
	BOOST_REQUIRE(l_AddDump[2] == 30);
	BOOST_REQUIRE(l_AddDump[3] == -2);

	for(int i = 4; i < 16; ++i)
		BOOST_REQUIRE(l_AddDump[i] == 0);
}

//BOOST_AUTO_TEST_CASE( AndTest )
//{
//	cpu.Init(std::move(AndTestData));
//	cpu.InterpretOp();	// ANDI
//	cpu.InterpretOp();	// AND	(inplace)
//	cpu.InterpretOp();	// AND
//	cpu.InterpretOp();	// TSTI
//	cpu.InterpretOp();	// TST
//	std::vector<UInt16> l_AndDump(cpu.DumpRegisters());
//
//}
//
//BOOST_AUTO_TEST_CASE( DivTest )
//{
//	cpu.Init(std::move(DivTestData));
//	cpu.InterpretOp();	// DIVI
//	cpu.InterpretOp();	// DIV	(inplace)
//	cpu.InterpretOp();	// DIV
//	std::vector<UInt16> l_DivDump(cpu.DumpRegisters());
//
//}
//
//BOOST_AUTO_TEST_CASE( MulTest )
//{
//	cpu.Init(std::move(MulTestData));
//	cpu.InterpretOp();	// MULI
//	cpu.InterpretOp();	// MUL	(inplace)
//	cpu.InterpretOp();	// MUL
//	std::vector<UInt16> l_MulDump(cpu.DumpRegisters());
//
//}
//
//BOOST_AUTO_TEST_CASE( OrTest )
//{
//	cpu.Init(std::move(OrTestData));
//	cpu.InterpretOp();	// ORI
//	cpu.InterpretOp();	// OR	(inplace)
//	cpu.InterpretOp();	// OR
//	std::vector<UInt16> l_OrDump(cpu.DumpRegisters());
//
//}
//
//BOOST_AUTO_TEST_CASE( SubTest )
//{
//	cpu.Init(std::move(SubTestData));
//	cpu.InterpretOp();	// SUBI
//	cpu.InterpretOp();	// SUB	(inplace)
//	cpu.InterpretOp();	// SUB
//	cpu.InterpretOp();	// CMPI
//	cpu.InterpretOp();	// CMP
//	std::vector<UInt16> l_SubDump(cpu.DumpRegisters());
//
//}
//
//BOOST_AUTO_TEST_CASE( XorTest )
//{
//	cpu.Init(std::move(XorTestData));
//	cpu.InterpretOp();	// XORI
//	cpu.InterpretOp();	// XOR	(inplace)
//	cpu.InterpretOp();	// XOR
//	std::vector<UInt16> l_XorDump(cpu.DumpRegisters());
//
//}

BOOST_AUTO_TEST_CASE( CallTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( JumpTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( PopTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( PushTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( StoreTest )
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_SUITE_END()
