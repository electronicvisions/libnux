#include <s2pp.h>
#include <libnux/unittest.h>

__vector uint8_t foo(__vector uint8_t* v)
{
	return *v;
}

void test_return_vector()
{
	libnux_testcase_begin("return_vector");
	__vector uint8_t in = vec_splat_u8(1);
	__vector uint8_t out;
	out = foo(&in);

	for (uint32_t i = 0; i < sizeof(__vector uint8_t); ++i) {
		libnux_test_equal(in[i], 1);
		libnux_test_equal(out[i], 1);
	}
	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_return_vector();
	libnux_test_summary();
	libnux_test_shutdown();
}
