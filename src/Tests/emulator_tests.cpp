#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emulator_tests.h"

BOOST_FIXTURE_TEST_SUITE( EmulatorTestSuite, EmulatorFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	// Test file error
	BOOST_REQUIRE(Emu.Init(ROM_PATH) & Emulator::FileError);

	// TODO : Test CPU error
	// TODO : Test GPU error
	// TODO : Test SPU error
}

BOOST_AUTO_TEST_SUITE_END()
