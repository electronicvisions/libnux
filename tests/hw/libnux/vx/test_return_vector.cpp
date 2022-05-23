#include <s2pp.h>
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

__vector uint8_t foo(__vector uint8_t* v)
{
	return *v;
}

void test_return_vector()
{
	testcase_begin("return_vector");
	__vector uint8_t in = vec_splat_u8(1);
	__vector uint8_t out;
	out = foo(&in);

	for (uint32_t i = 0; i < sizeof(__vector uint8_t); ++i) {
		test_equal(in[i], 1);
		test_equal(out[i], 1);
	}
	testcase_end();
}

void start()
{
	test_init();
	test_return_vector();
	test_summary();
	test_shutdown();
}
