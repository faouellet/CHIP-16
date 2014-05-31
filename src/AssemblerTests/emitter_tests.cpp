#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emitter_tests.h"

#include <fstream>

BOOST_FIXTURE_TEST_SUITE(EmitterTestSuite, EmitterFixture)

BOOST_AUTO_TEST_CASE( ArithmeticTest )
{
	Emitter.EmitAdd();
	Emitter.EmitAddImm();
	Emitter.EmitAddInp();

	Emitter.EmitAnd();
	Emitter.EmitAndImm();
	Emitter.EmitAndInp();

	Emitter.EmitARShift();
	Emitter.EmitARShiftImm();

	Emitter.EmitCmp();
	Emitter.EmitCmpImm();

	Emitter.EmitDiv();
	Emitter.EmitDivImm();
	Emitter.EmitDivInp();

	Emitter.EmitLLShift();
	Emitter.EmitLLShiftImm();

	Emitter.EmitLRShift();
	Emitter.EmitLRShiftImm();

	Emitter.EmitMul();
	Emitter.EmitMulImm();
	Emitter.EmitMulInp();

	Emitter.EmitOr();
	Emitter.EmitOrImm();
	Emitter.EmitOrInp();

	Emitter.EmitSub();
	Emitter.EmitSubImm();
	Emitter.EmitSubInp();

	Emitter.EmitTst();
	Emitter.EmitTstImm();

	Emitter.EmitXor();
	Emitter.EmitXorImm();
	Emitter.EmitXorInp();

	Emitter.EmitToFile("test.c16");

	std::ifstream l_TestFileStream(TestFilename);

	BOOST_REQUIRE(l_TestFileStream.is_open());


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
