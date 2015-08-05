#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Emitter
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "emitter_tests.h"

BOOST_FIXTURE_TEST_SUITE(EmitterTestSuite, EmitterFixture)

BOOST_AUTO_TEST_CASE( ArithmeticTest )
{
	Emit.EmitAdd(0x01, 0x02, 0x03);
	Emit.EmitAddImm(0x01, 0x00FF);
	Emit.EmitAddInp(0x01, 0x02);

	Emit.EmitAnd(0x05, 0x04, 0x06);
	Emit.EmitAndImm(0x01, 0xAA00);
	Emit.EmitAndInp(0x03, 0x02);

	Emit.EmitARShift(0x01, 0x02);
	Emit.EmitARShiftImm(0x03, 0x02);

	Emit.EmitCmp(0x05, 0x06);
	Emit.EmitCmpImm(0x04, 0xFF00);

	Emit.EmitDiv(0x01, 0x02, 0x03);
	Emit.EmitDivImm(0x01, 0x00FF);
	Emit.EmitDivInp(0x01, 0x02);

	Emit.EmitLLShift(0x01, 0x02);
	Emit.EmitLLShiftImm(0x01, 0x02);

	Emit.EmitLRShift(0x01, 0x02);
	Emit.EmitLRShiftImm(0x02, 0x04);

	Emit.EmitMul(0x01, 0x02, 0x03);
	Emit.EmitMulImm(0x01, 0x00FF);
	Emit.EmitMulInp(0x01, 0x02);

	Emit.EmitOr(0x01, 0x02, 0x03);
	Emit.EmitOrImm(0x01, 0x00FF);
	Emit.EmitOrInp(0x03, 0x02);

	Emit.EmitSub(0x01, 0x02, 0x03);
	Emit.EmitSubImm(0x01, 0x00EE);
	Emit.EmitSubInp(0x01, 0x02);

	Emit.EmitTst(0x03, 0x02);
	Emit.EmitTstImm(0x01, 0xFFFF);

	Emit.EmitXor(0x01, 0x02, 0x03);
	Emit.EmitXorImm(0x01, 0xFF00);
	Emit.EmitXorInp(0x01, 0x02);

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), ArithmeticTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], ArithmeticTestData[i]);
}

BOOST_AUTO_TEST_CASE( StackTest )
{
	Emit.EmitPush(0x01);
	Emit.EmitPop(0x02);
	Emit.EmitPushAll();
	Emit.EmitPopAll();
	Emit.EmitPushF();
	Emit.EmitPopF();

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), StackTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], StackTestData[i]);
}

BOOST_AUTO_TEST_CASE( MemoryTest )
{
	Emit.EmitLDIImm(0x01, 0x00FF);
	Emit.EmitLDISP(0x00FF);

	Emit.EmitLDM(0x01, 0x02);
	Emit.EmitLDMImm(0x01, 0x00FF);

	Emit.EmitMov(0x01, 0x02);

	Emit.EmitStm(0x01, 0x02);
	Emit.EmitStmImm(0x01, 0x00FF);

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), MemoryTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], MemoryTestData[i]);
}

BOOST_AUTO_TEST_CASE( JumpTest )
{
	Emit.EmitJmp(0x00FF);
	Emit.EmitJx(0x01, 0x00FF);
	Emit.EmitJme(0x01, 0x02, 0x00FF);
	Emit.EmitJmpInd(0x01);

	Emit.EmitCall(0x01);
	Emit.EmitCallImm(0x00FF);
	Emit.EmitRet();
	Emit.EmitCx(0x01, 0x00FF);

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), JumpTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
	{
		if (l_TestData[i] != JumpTestData[i])
			int k = 0;
		BOOST_REQUIRE_EQUAL(l_TestData[i], JumpTestData[i]);
	}
}

BOOST_AUTO_TEST_CASE( PaletteTest )
{
	Emit.EmitPal(0x01);
	Emit.EmitPalImm(0x00FF);

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), PaletteTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], PaletteTestData[i]);
}

BOOST_AUTO_TEST_CASE( MiscTest )
{
	Emit.EmitNop();
	Emit.EmitCls();
	Emit.EmitVBlnk();
	Emit.EmitBgc(0x01);
	Emit.EmitSpr(0x00FF);
	Emit.EmitDrw(0x01, 0x02, 0x03);
	Emit.EmitDrwImm(0x01, 0x02, 0x00FF);
	Emit.EmitRnd(0x01, 0x00FF);
	Emit.EmitFlip(0x01, 0x01);

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), MiscTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
		BOOST_REQUIRE_EQUAL(l_TestData[i], MiscTestData[i]);
}

BOOST_AUTO_TEST_CASE( SoundTest )
{
	Emit.EmitSnd0();
	Emit.EmitSnd1(0x00FF);
	Emit.EmitSnd2(0x00FF);
	Emit.EmitSnd3(0x00FF);
	Emit.EmitSnp(0x01, 0x00FF);
	//Emitter.EmitSng();

	Emit.EmitToFile(TestFilename);

	std::vector<Utils::UInt8> l_TestData(ReadFile());

	BOOST_REQUIRE_EQUAL(l_TestData.size(), SoundTestData.size());

	for (unsigned i = 0; i < l_TestData.size(); ++i)
	{
		if (l_TestData[i] != SoundTestData[i])
			int k = 0;
		BOOST_REQUIRE_EQUAL(l_TestData[i], SoundTestData[i]);
	}
}

BOOST_AUTO_TEST_SUITE_END()
