#include <s2pp.h>
#include "libnux/dls.h"
#include "libnux/unittest.h"

void test_read_g_after_write_v(void)
{
	libnux_testcase_begin(__func__);

	// Write with VX
	__vector uint8_t vec = vec_splat_u8(0xff);

	// Read with GPP
	libnux_test_equal(vec[0], 0xff);

	libnux_testcase_end();
}

void test_read_v_after_write_g(void)
{
	libnux_testcase_begin(__func__);

	__vector uint8_t vec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Write with GPP
	for (uint32_t i = 0; i < 16; i++) {
		vec[i] = 1;
	}

	__vector uint8_t temp;
	__vector uint8_t copy;

	// Read from VX
	// clang-format off
	asm volatile(
		"fxvlax %[temp], %[src_ptr]\n"
		"fxvstax %[temp], %[tgt_ptr]\n"
		"sync"
		: [temp] "=&kv"(temp), [tgt_ptr] "=m"(copy)
		: [src_ptr] "m"(vec));
	// clang-format on

	// Check the copy
	for (uint32_t i = 0; i < 16; i++) {
		libnux_test_equal(copy[i], 1);
	}

	libnux_testcase_end();
}

void test_write_g_after_read_v(void)
{
	libnux_testcase_begin(__func__);

	__vector uint8_t vec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	__vector uint8_t copy;

	// Read with VX
	// clang-format off
	asm volatile(
		// Read a correlation measurement which takes ~300 cycles
		"fxvinx %[copy], %[dls_causal_base], %[index]\n"
		// Load the vector from memory into a register
		"fxvlax %[copy], %[ptr]"
		: [copy] "=kv"(copy)
		: [dls_causal_base] "b"(dls_causal_base), [index] "r"(0), [ptr] "Z"(vec)
		: /* no clobbering */);
	// clang-format on

	// Write with GPP
	for (uint32_t i = 0; i < 16; i++) {
		vec[i] = 1;
	}

	// Check the copy
	for (uint32_t i = 0; i < 16; i++) {
		libnux_test_equal(copy[i], 0);
	}

	libnux_testcase_end();
}

void test_write_v_after_read_g(void)
{
	libnux_testcase_begin(__func__);

	__vector uint8_t vec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Read with GPP
	for (uint32_t i = 0; i < 16; i++) {
		libnux_test_equal(vec[i], 0);
	}

	// Write with VX
	vec = vec_splat_u8(1);

	// Read with GPP
	for (uint32_t i = 0; i < 16; i++) {
		libnux_test_equal(vec[i], 1);
	}

	libnux_testcase_end();
}

int32_t start(void)
{
	libnux_test_init();
	test_read_g_after_write_v();
	test_read_v_after_write_g();
	test_write_g_after_read_v();
	test_write_v_after_read_g();
	libnux_test_summary();
	libnux_test_shutdown();

	return 0;
}
