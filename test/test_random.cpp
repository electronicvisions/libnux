#include "libnux/unittest.h"
#include "libnux/random.h"

void test_initialize(size_t row, size_t seed)
{
	init_random_generator(row, seed);
	make_rand_calls(row, 10);
}

#define VECTOR_RAND_TEST(type, func_name)\
void test_##func_name(size_t row)\
{\
	libnux_testcase_begin(""#func_name"");\
\
	__vector type test_vec;\
	test_vec = func_name(row);\
\
	uint32_t additor = 0;\
	for(size_t i = 0; i < sizeof(__vector type) / sizeof(type); i++) {\
		additor += test_vec[i];\
	}\
\
	libnux_test_true(additor > 0);\
\
	libnux_testcase_end();\
}

VECTOR_RAND_TEST(uint8_t, get_rand_u8)
VECTOR_RAND_TEST(uint16_t, get_rand_u16)
#undef VECTOR_RAND_TEST

#define VECTOR_RAND_BITWISE_TEST(type, func_name)\
void test_##func_name(size_t row, size_t shift)\
{\
	libnux_testcase_begin(""#func_name"");\
\
	__vector type test_vec;\
	test_vec = func_name(row, shift);\
\
	asm volatile(\
		"sync"\
	);\
\
	for(size_t i = 0; i < sizeof(__vector type) / sizeof(type); i++) {\
		libnux_test_true(test_vec[i] < (1 << shift));\
	}\
\
	libnux_testcase_end();\
}

VECTOR_RAND_BITWISE_TEST(uint8_t, get_rand_bitwise_u8)
VECTOR_RAND_BITWISE_TEST(uint16_t, get_rand_bitwise_u16)
#undef VECTOR_RAND_BITWISE_TEST

void start()
{
	test_initialize(0, 1);
	libnux_test_init();
	test_get_rand_u8(0);
	test_get_rand_u16(0);
	test_get_rand_bitwise_u8(0, 4);
	test_get_rand_bitwise_u16(0, 9);
	libnux_test_summary();
	libnux_test_shutdown();
}
