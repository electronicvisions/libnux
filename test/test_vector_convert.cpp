#include "libnux/unittest.h"
#include "libnux/vector_convert.h"

void test_uint8_to_int8()
{
	libnux_testcase_begin("uint8_to_int8");

	__vector uint8_t a = vec_splat_u8(0);
	auto const ra = libnux::uint8_to_int8(a);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		libnux_test_equal(ra[index], -128);
	}

	__vector uint8_t b = vec_splat_u8(255);
	auto const rb = libnux::uint8_to_int8(b);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		libnux_test_equal(rb[index], 127);
	}

	__vector uint8_t c = vec_splat_u8(123);
	auto const rc = libnux::uint8_to_int8(c);
	for (uint32_t index = 0; index < sizeof(__vector int8_t); index++) {
		libnux_test_equal(rc[index], -128 + 123);
	}

	libnux_testcase_end();
}

void start(void)
{
	libnux_test_init();
	test_uint8_to_int8();
	libnux_test_summary();
	libnux_test_shutdown();
}
