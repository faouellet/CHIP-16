#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE Assembler
#endif
#endif

#include <boost/test/unit_test.hpp>

#include "assembler_tests.h"

BOOST_FIXTURE_TEST_SUITE( AssemblerTestSuite, AssemblerFixture )

BOOST_AUTO_TEST_CASE( DummyTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
