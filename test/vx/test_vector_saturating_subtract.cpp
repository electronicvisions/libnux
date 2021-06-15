#include "libnux/vx/unittest.h"
#include "libnux/vx/vector_math.h"

using namespace libnux::vx;

void test_saturating_subtract()
{
	testcase_begin("saturating_subtract");

	{
		__vector int8_t a = vec_splat_s8(-127);
		__vector int8_t b = vec_splat_s8(2);
		auto const rb = saturating_subtract(a, b);
		for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
			test_equal(rb[index], -128);
		}
	}

	{
		__vector int8_t a = vec_splat_s8(1);
		__vector int8_t b = vec_splat_s8(-127);
		auto const rb = saturating_subtract(a, b);
		for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
			test_equal(rb[index], 127);
		}
	}

	testcase_end();
}

void start(void)
{
	test_init();
	test_saturating_subtract();
	test_summary();
	test_shutdown();
}
