#include "libnux/vx/unittest.h"
#include "libnux/vx/vector_convert.h"

using namespace libnux::vx;

void test_uint8_to_int8()
{
	testcase_begin("uint8_to_int8");

	__vector uint8_t a = vec_splat_u8(0);
	auto const ra = uint8_to_int8(a);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		test_equal(ra[index], -128);
	}

	__vector uint8_t b = vec_splat_u8(255);
	auto const rb = uint8_to_int8(b);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		test_equal(rb[index], 127);
	}

	__vector uint8_t c = vec_splat_u8(123);
	auto const rc = uint8_to_int8(c);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		test_equal(rc[index], -128 + 123);
	}

	testcase_end();
}

void start(void)
{
	test_init();
	test_uint8_to_int8();
	test_summary();
	test_shutdown();
}
