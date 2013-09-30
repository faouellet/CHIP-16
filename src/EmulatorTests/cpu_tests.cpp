#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "cpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( CPUTestSuite, CPUFixture )

BOOST_AUTO_TEST_CASE( ArithmeticTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( CallTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( JumpTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PopTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( PushTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( ShiftTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( StoreTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
