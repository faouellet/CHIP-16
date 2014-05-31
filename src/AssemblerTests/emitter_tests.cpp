#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emitter_tests.h"

BOOST_FIXTURE_TEST_SUITE(EmitterTestSuite, EmitterFixture)

BOOST_AUTO_TEST_CASE( ArithmeticTest )
{
	Emitter.EmitAdd(0x01, 0x02, 0x03);
	Emitter.EmitAddImm(0x01, 0x00FF);
	Emitter.EmitAddInp(0x01, 0x02);

	Emitter.EmitAnd(0x05, 0x04, 0x06);
	Emitter.EmitAndImm(0x01, 0xAA00);
	Emitter.EmitAndInp(0x03, 0x02);

	Emitter.EmitARShift(0x01, 0x02);
	Emitter.EmitARShiftImm(0x03, 0x02);

	Emitter.EmitCmp(0x05, 0x06);
	Emitter.EmitCmpImm(0x03, 0x02);

	Emitter.EmitDiv(0x01, 0x02, 0x03);
	Emitter.EmitDivImm(0x01, 0x00FF);
	Emitter.EmitDivInp(0x01, 0x02);

	Emitter.EmitLLShift(0x01, 0x02);
	Emitter.EmitLLShiftImm(0x01, 0x02);

	Emitter.EmitLRShift(0x01, 0x02);
	Emitter.EmitLRShiftImm(0x01, 0x02);

	Emitter.EmitMul(0x01, 0x02, 0x03);
	Emitter.EmitMulImm(0x01, 0x00FF);
	Emitter.EmitMulInp(0x01, 0x02);

	Emitter.EmitOr(0x01, 0x02, 0x03);
	Emitter.EmitOrImm(0x01, 0x00FF);
	Emitter.EmitOrInp(0x01, 0x02);

	Emitter.EmitSub(0x01, 0x02, 0x03);
	Emitter.EmitSubImm(0x01, 0x00EE);
	Emitter.EmitSubInp(0x01, 0x02);

	Emitter.EmitTst(0x03, 0x02);
	Emitter.EmitTstImm(0x01, 0xFFFF);

	Emitter.EmitXor(0x01, 0x02, 0x03);
	Emitter.EmitXorImm(0x01, 0xFF00);
	Emitter.EmitXorInp(0x01, 0x02);

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), ArithmeticTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], ArithmeticTestData[i]);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( JumpTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PaletteTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( MiscTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( SoundTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
