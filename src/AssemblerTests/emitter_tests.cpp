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
	Emitter.EmitPush(0x01);
	Emitter.EmitPop(0x02);
	Emitter.EmitPushAll();
	Emitter.EmitPopAll();
	Emitter.EmitPushF();
	Emitter.EmitPopF();

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), StackTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], StackTestData[i]);
}

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	Emitter.EmitLDIImm(0x01, 0x00FF);
	Emitter.EmitLDISP(0x00FF);

	Emitter.EmitLDM(0x01, 0x02);
	Emitter.EmitLDMImm(0x01, 0x00FF);

	Emitter.EmitMov(0x01, 0x02);

	Emitter.EmitStm(0x01, 0x02);
	Emitter.EmitStmImm(0x01, 0x00FF);

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), MemoryTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], MemoryTestData[i]);
}

BOOST_AUTO_TEST_CASE( JumpTest )
{
	Emitter.EmitJmp(0x00FF);
	Emitter.EmitJx(0x01, 0x00FF);
	Emitter.EmitJme(0x01, 0x02, 0x00FF);
	Emitter.EmitJmpInd();

	Emitter.EmitCall(0x00FF);
	Emitter.EmitCallImm(0x00FF);
	Emitter.EmitRet();
	Emitter.EmitCx(0x01, 0x00FF);

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), JumpTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], JumpTestData[i]);
}

BOOST_AUTO_TEST_CASE( PaletteTest )
{
	Emitter.EmitPal(0x01);
	Emitter.EmitPalImm(0x00FF);

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), PaletteTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], PaletteTestData[i]);
}

BOOST_AUTO_TEST_CASE( MiscTest )
{
	Emitter.EmitNop();
	Emitter.EmitCls();
	Emitter.EmitVBlnk();
	Emitter.EmitBgc(0x01);
	Emitter.EmitSpr(0x00FF);
	Emitter.EmitDrw(0x01, 0x02, 0x03);
	Emitter.EmitDrwImm(0x01, 0x02, 0x00FF);
	Emitter.EmitRnd(0x01, 0x00FF);
	Emitter.EmitFlip(0x01, 0x01);

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), MiscTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], MiscTestData[i]);
}

BOOST_AUTO_TEST_CASE( SoundTest )
{
	Emitter.EmitSnd0();
	Emitter.EmitSnd1();
	Emitter.EmitSnd2();
	Emitter.EmitSnd3();
	Emitter.EmitSnp();
	Emitter.EmitSng();

	Emitter.EmitToFile(TestFilename);

	std::vector<UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), SoundTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], SoundTestData[i]);
}

BOOST_AUTO_TEST_SUITE_END()
