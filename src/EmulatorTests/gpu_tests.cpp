#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "gpu_tests.h"

BOOST_FIXTURE_TEST_SUITE( GPUTestSuite, GPUFixture )

BOOST_AUTO_TEST_CASE( InitTest )
{
	std::vector<UInt32> l_Palette(Gpu.DumpColors());
	BOOST_REQUIRE_EQUAL(l_Palette[0], 0x000000);
	BOOST_REQUIRE_EQUAL(l_Palette[1], 0x000000);
	BOOST_REQUIRE_EQUAL(l_Palette[2], 0x888888);
	BOOST_REQUIRE_EQUAL(l_Palette[3], 0x3239BF);
	BOOST_REQUIRE_EQUAL(l_Palette[4], 0xAE7ADE);
	BOOST_REQUIRE_EQUAL(l_Palette[5], 0x213D4C);
	BOOST_REQUIRE_EQUAL(l_Palette[6], 0x255F90);
	BOOST_REQUIRE_EQUAL(l_Palette[7], 0x5294E4);
	BOOST_REQUIRE_EQUAL(l_Palette[8], 0x79D9EA);
	BOOST_REQUIRE_EQUAL(l_Palette[9], 0x3B7A53);
	BOOST_REQUIRE_EQUAL(l_Palette[10], 0x4AD5AB);
	BOOST_REQUIRE_EQUAL(l_Palette[11], 0x382E25);
	BOOST_REQUIRE_EQUAL(l_Palette[12], 0x7F4600);
	BOOST_REQUIRE_EQUAL(l_Palette[13], 0xCCAB68);
	BOOST_REQUIRE_EQUAL(l_Palette[14], 0xE4DEBC);
	BOOST_REQUIRE_EQUAL(l_Palette[15], 0xFFFFFF);

	std::vector<UInt32> l_ScreenBuffer(Gpu.DumpScreenBuffer());

	for(int i = 0; i < 240; ++i)
		for(int j = 0; j < 320; ++j)
			BOOST_REQUIRE_EQUAL(l_ScreenBuffer[i * 240 + j], 0);
}

BOOST_AUTO_TEST_CASE( DrawTest )
{
	std::vector<UInt8> l_Sprite(100, 3);

	Gpu.Draw(10, 10, l_Sprite);
	
	std::vector<UInt32> l_ScreenBuffer(Gpu.DumpScreenBuffer());
	
	for(int i = 0; i < 10; ++i)
	{
		for(int j = 0; j < 320; ++j)
		{
			if(j < 10)
				BOOST_REQUIRE_EQUAL(l_ScreenBuffer[i * 320 + j], 3);
			else
				BOOST_REQUIRE_EQUAL(l_ScreenBuffer[i * 320 + j], 0);
		}
	}

	for(int i = 10; i < 240; ++i)
		for(int j = 0; j < 320; ++j)
			BOOST_REQUIRE_EQUAL(l_ScreenBuffer[i * 320 + j], 0);
}

BOOST_AUTO_TEST_CASE( PaletteTest )
{
	// TODO
}

BOOST_AUTO_TEST_CASE( SpriteTest )
{
	Gpu.SetSpriteDimensions(10, 10);
	BOOST_REQUIRE_EQUAL(Gpu.SpriteHeight(), 10);
	BOOST_REQUIRE_EQUAL(Gpu.SpriteWidth(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
