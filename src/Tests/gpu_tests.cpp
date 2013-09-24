#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "gpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( GPUTestSuite, GPUFixture )

BOOST_AUTO_TEST_CASE( DummyTest )
{
	// TODO
}

BOOST_AUTO_TEST_SUITE_END()
