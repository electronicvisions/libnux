#include "libnux/unittest.h"
#include "libnux/vector_helper.h"

#define VECTOR_SHIFT_TEST(type, select, func_name)\
void test_##func_name(size_t shift, type start, type expectation)\
{\
	libnux_testcase_begin(""#func_name"");\
\
	__vector type test_vec = vec_splat_##select(start);\
\
	func_name(test_vec, shift);\
\
	libnux_test_equal(test_vec[0], expectation);\
\
	libnux_testcase_end();\
}

VECTOR_SHIFT_TEST(uint8_t, u8, shift_vector_left_u8)
VECTOR_SHIFT_TEST(uint8_t, u8, shift_vector_right_u8)
VECTOR_SHIFT_TEST(uint16_t, u16, shift_vector_left_u16)
VECTOR_SHIFT_TEST(uint16_t, u16, shift_vector_right_u16)
#undef VECTOR_SHIFT_TEST

void start()
{
	libnux_test_init();
	test_shift_vector_left_u8(4, 0x0f, 0xf0);
	test_shift_vector_right_u8(4, 0xf0, 0x0f);
	test_shift_vector_left_u16(8, 0x00f0, 0xf000);
	test_shift_vector_right_u16(8, 0x0f00, 0x000f);
	libnux_test_summary();
	libnux_test_shutdown();
}
