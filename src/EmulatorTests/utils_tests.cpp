#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "utils.h"

BOOST_AUTO_TEST_CASE( StaticConditionalTest )
{
	Utils::IfElse<0==0, char, double> l_True;
	Utils::IfElse<0!=0, char, double> l_False;

	BOOST_REQUIRE( true );
	// BOOST_REQUIRE(sizeof(l_True::type) == sizeof(char));
	// BOOST_REQUIRE(sizeof(l_False::type) == sizeof(double));
}
