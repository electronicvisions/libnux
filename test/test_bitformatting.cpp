#include "libnux/unittest.h"
#include "libnux/bitformatting.h"

void test_reverse_byte()
{
	uint8_t byte_1 = 170;
	libnux_test_equal(reverse_byte(byte_1), 85);

	uint8_t byte_2 = 195;
	libnux_test_equal(reverse_byte(byte_2), 195);

	uint8_t byte_3 = 0;
	libnux_test_equal(reverse_byte(byte_3), 0);
}

#ifndef LIBNUX_DLS_VERSION_VX
void test_vector_reverse_bytes()
{
	__vector uint8_t test_vector;

	for (uint8_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_vector[i] = i;
	}

	vector_reverse_bytes(&test_vector);

	for (uint8_t i = 0; i < sizeof(__vector uint8_t); i++) {
		libnux_test_equal(test_vector[i], reverse_byte(i));
	}
}
#endif

void start()
{
	libnux_test_init();
	test_reverse_byte();
#ifndef LIBNUX_DLS_VERSION_VX
	test_vector_reverse_bytes();
#endif
	libnux_test_summary();
	libnux_test_shutdown();
}
