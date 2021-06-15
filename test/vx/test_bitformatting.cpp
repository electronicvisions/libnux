#include "libnux/vx/unittest.h"
#include "libnux/vx/bitformatting.h"

using namespace libnux::vx;

void test_reverse_byte()
{
	uint8_t byte_1 = 170;
	test_equal(reverse_byte(byte_1), 85);

	uint8_t byte_2 = 195;
	test_equal(reverse_byte(byte_2), 195);

	uint8_t byte_3 = 0;
	test_equal(reverse_byte(byte_3), 0);
}

void test_vector_reverse_bytes()
{
	__vector uint8_t test_vector;

	for (uint8_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_vector[i] = i;
	}

	vector_reverse_bytes(&test_vector);

	for (uint8_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(test_vector[i], reverse_byte(i));
	}
}

void start()
{
	test_init();
	test_reverse_byte();
	test_vector_reverse_bytes();
	test_summary();
	test_shutdown();
}
