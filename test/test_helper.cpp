#include <array>

#include <s2pp.h>
#include "libnux/helper.h"
#include "libnux/unittest.h"

void test_abs_diff()
{
	uint8_t u8bit_1 = 150;
	uint8_t u8bit_2 = 15;
	libnux_test_equal(abs_diff(u8bit_1, u8bit_2), static_cast<uint8_t>(135));

	uint16_t u16bit_1 = 300;
	uint16_t u16bit_2 = 3000;
	libnux_test_equal(abs_diff(u16bit_1, u16bit_2), static_cast<uint16_t>(2700));

	uint32_t u32bit_1 = 100;
	uint32_t u32bit_2 = 100;
	libnux_test_equal(abs_diff(u32bit_1, u32bit_2), static_cast<uint32_t>(0));

	int8_t s8bit_1 = -10;
	int8_t s8bit_2 = -20;
	libnux_test_equal(abs_diff(s8bit_1, s8bit_2), static_cast<int8_t>(10));

	int16_t s16bit_1 = -1000;
	int16_t s16bit_2 = -100;
	libnux_test_equal(abs_diff(s16bit_1, s16bit_2), static_cast<int16_t>(900));
}

void test_find_max_amount()
{
	std::array<uint8_t, 10> array_1{4, 17, 193, 4, 193, 10, 5, 3, 4, 0};
	libnux_test_equal(find_max_amount(array_1), static_cast<uint8_t>(4));

	std::array<uint16_t, 5> array_2{1600, 300, 1600, 300, 1600};
	libnux_test_equal(find_max_amount(array_2), static_cast<uint16_t>(1600));
}

void start()
{
	libnux_test_init();
	test_abs_diff();
	test_find_max_amount();
	libnux_test_summary();
	libnux_test_shutdown();
}
