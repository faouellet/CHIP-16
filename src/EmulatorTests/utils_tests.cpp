#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "utils.h"

#include <limits>

BOOST_AUTO_TEST_CASE( StaticConditionalTest )
{
	BOOST_REQUIRE(sizeof(Utils::IfElse<true, char, double>::type) == sizeof(char));
	BOOST_REQUIRE(sizeof(Utils::IfElse<false, char, double>::type) == sizeof(double));
}

// TODO : test Error class
BOOST_AUTO_TEST_CASE( TypeSizeTest )
{
	// Test number of bytes
	BOOST_REQUIRE(sizeof(Utils::UInt_<1>) <= 1);
	BOOST_REQUIRE(sizeof(Utils::UInt_<2>) <= 2);
	BOOST_REQUIRE(sizeof(Utils::UInt_<4>) <= 4);
	BOOST_REQUIRE(sizeof(Utils::UInt_<8>) <= 8);

	// Test sign
	BOOST_REQUIRE(std::numeric_limits<Utils::UInt_<1>::type>::min() == 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::UInt_<2>::type>::min() == 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::UInt_<4>::type>::min() == 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::UInt_<8>::type>::min() == 0);
		
	// Test number of bytes
	BOOST_REQUIRE(sizeof(Utils::Int_<1>) <= 1);
	BOOST_REQUIRE(sizeof(Utils::Int_<2>) <= 2);
	BOOST_REQUIRE(sizeof(Utils::Int_<4>) <= 4);
	BOOST_REQUIRE(sizeof(Utils::Int_<8>) <= 8);
		
	// Test sign
	BOOST_REQUIRE(std::numeric_limits<Utils::Int_<1>::type>::min() < 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::Int_<2>::type>::min() < 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::Int_<4>::type>::min() < 0);
	BOOST_REQUIRE(std::numeric_limits<Utils::Int_<8>::type>::min() < 0);
}

BOOST_AUTO_TEST_CASE( LeftShiftTest )
{
	// TODO : more ?
	Utils::LeftShift l_LS;
	BOOST_REQUIRE(l_LS(1,1) == 2);
}

BOOST_AUTO_TEST_CASE( ArithmeticRightShiftTest )
{
	Utils::ArithmeticRightShift l_ARS;
	BOOST_REQUIRE(l_ARS(0,2) == 0);
	BOOST_REQUIRE(l_ARS(1,2) == 0);
	BOOST_REQUIRE(l_ARS(-1,16) == std::numeric_limits<Utils::UInt16>::max());
}

BOOST_AUTO_TEST_CASE( LogicalRightShiftTest )
{
	Utils::LogicalRightShift l_LRS;
	BOOST_REQUIRE(l_LRS(0,2) == 0);
	BOOST_REQUIRE(l_LRS(1,2) == 0);
	BOOST_REQUIRE(l_LRS(-1,16) == 0);
}
