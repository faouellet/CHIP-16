#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "emulator_demos.h"
#include "utils.h"

BOOST_FIXTURE_TEST_SUITE( EmulatorDemoSuite, EmulatorDemoFixture )

BOOST_AUTO_TEST_CASE( DummyTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
