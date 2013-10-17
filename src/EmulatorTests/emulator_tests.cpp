#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emulator_tests.h"
#include "utils.h"

BOOST_FIXTURE_TEST_SUITE( EmulatorTestSuite, EmulatorTestFixture )

// TODO : Check the results ?? do something more ??

BOOST_AUTO_TEST_CASE( InitTest )
{
	BOOST_REQUIRE( true );

	// Test file error
	BOOST_REQUIRE(Emu.Init("") & Utils::FileError);

	// TODO : Test CPU errors
	// TODO : Test GPU errors
	// TODO : Test SPU errors

	// Test standard ROM
	BOOST_REQUIRE_EQUAL(Emu.Init(ROM_PATH+"/AdsrTest/AdsrTest.c16"), Utils::NoError);
}

BOOST_AUTO_TEST_CASE( AdsrTest )
{
	/*Emu.Init(ROM_PATH + "/AdsrTest/AdsrTest.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( BC_TestRom )
{
	/*BOOST_REQUIRE_EQUAL(Emu.Init(ROM_PATH + "/BC_TestRom/BC_TestRom.c16"), Utils::NoError);
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( CollisionTest )
{
	//Emu.Init(ROM_PATH + "/CollisionTest/CollisionTest.c16");
	Emu.Init(ROM_PATH + "/Mandel/Mandel.c16");
	Emu.Emulate();
}

BOOST_AUTO_TEST_CASE( flip_test )
{
	/*Emu.Init(ROM_PATH + "/flip_test/flip_test.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( PadTest )
{
	/*Emu.Init(ROM_PATH + "/PadTest/PadTest.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( PaleteTest )
{
	/*Emu.Init(ROM_PATH + "/PaleteTest/PaleteTest.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( PaletteFlip )
{
	/*Emu.Init(ROM_PATH + "/PaletteFlip/paletteflip.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_CASE( SoundTest )
{
	/*Emu.Init(ROM_PATH + "/SoundTest/SoundTest.c16");
	Emu.Emulate();*/
}

BOOST_AUTO_TEST_SUITE_END()
