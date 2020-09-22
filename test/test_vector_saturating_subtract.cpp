#include "libnux/unittest.h"
#include "libnux/vector_math.h"

void test_saturating_subtract()
{
	libnux_testcase_begin("saturating_subtract");

	{
		__vector int8_t a = vec_splat_s8(-127);
		__vector int8_t b = vec_splat_s8(2);
		auto const rb = libnux::saturating_subtract(a, b);
		for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
			libnux_test_equal(rb[index], -128);
		}
	}

	{
		__vector int8_t a = vec_splat_s8(1);
		__vector int8_t b = vec_splat_s8(-127);
		auto const rb = libnux::saturating_subtract(a, b);
		for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
			libnux_test_equal(rb[index], 127);
		}
	}

	libnux_testcase_end();
}

void start(void)
{
	libnux_test_init();
	test_saturating_subtract();
	libnux_test_summary();
	libnux_test_shutdown();
}
