#include <s2pp.h>
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

// some function doing something with vector registers
uint8_t get_splatted_value(uint8_t const value)
{
	__vector uint8_t vec = vec_splat_u8(value);
	return vec[13];
}

volatile uint8_t value1= 1;
volatile uint8_t value2 = 2;

void start()
{
	test_init();
	testcase_begin("vector register reusage");

	// set vec to value1
	__vector uint8_t vec;
	asm volatile(
		"fxvsplatb %[vec], %[value]\n"
		: [vec] "=qv" (vec)
		: [value] "r" (value1) :
	);

	// do something in another function with vector registers
	uint8_t intermediate = get_splatted_value(value2);

	// assign vec = vec + 0, expected result is value1 from above
	__vector uint8_t tmp;
	asm volatile(
		"fxvsplatb %[tmp], %[zero]\n"
		"fxvaddbm %[vec], %[vec], %[tmp]\n"
		: [vec] "+qv" (vec),
		  [tmp] "=qv" (tmp)
		: [zero] "r" (uint32_t(0))
		:
	);

	uint8_t expected_value1 = vec[7];

	test_equal(intermediate, value2);
	// expect value1 after execution of nested function, since nothing should have changed
	test_equal(expected_value1, value1);
	testcase_end();

	test_summary();
	test_shutdown();
}
