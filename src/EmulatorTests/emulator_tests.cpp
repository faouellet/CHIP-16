#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emulator_tests.h"
#include "utils.h"

BOOST_FIXTURE_TEST_SUITE( EmulatorTestSuite, EmulatorFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	BOOST_REQUIRE( true );

	// Test file error
	// BOOST_REQUIRE(Emu.Init(ROM_PATH) & Utils::FileError);

	// TODO : Test CPU error
	// TODO : Test GPU error
	// TODO : Test SPU error
}

BOOST_AUTO_TEST_CASE( AdsrTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( BC_TestRom )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( CollisionTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( flip_test )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PadTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PaleteTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PaletteFlip )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( SoundTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
