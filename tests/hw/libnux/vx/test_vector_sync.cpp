#include <s2pp.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void test_read_g_after_write_v(void)
{
	testcase_begin(__func__);

	// Write with VX
	__vector uint8_t vec = vec_splat_u8(0xff);

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(vec[i], 0xff);
	}

	testcase_end();
}

void test_read_v_after_write_g(void)
{
	testcase_begin(__func__);

	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Write with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		vec[i] = 1;
	}

	__vector uint8_t temp;
	__vector uint8_t copy;

	// Read from VX
	// clang-format off
	asm volatile(
		"fxvlax %[temp], %y[src_ptr]\n"
		"fxvstax %[temp], %y[tgt_ptr]\n"
		"sync"
		: [temp] "=&qv" (temp),
		  [tgt_ptr] "=Z"(copy)
		: [src_ptr] "Z"(vec)
	);
	// clang-format on

	// Check the copy
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(copy[i], 1);
	}

	testcase_end();
}

void test_write_g_after_read_v(void)
{
	testcase_begin(__func__);

	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	__vector uint8_t copy;

	// Read with VX
	// clang-format off
	asm volatile(
		// Read a correlation measurement which takes ~300 cycles
		"fxvinx %[copy], %[dls_causal_base], %[index]\n"
		// Load the vector from memory into a register
		"fxvlax %[copy], %y[ptr]\n"
		"sync"
		: [copy] "=qv" (copy)
		: [dls_causal_base] "b"(dls_causal_base),
		  [index] "r"(0),
		  [ptr] "Z"(vec)
		: /* no clobbering */
	);
	// clang-format on

	// Write with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		vec[i] = 1;
	}

	// Check the copy
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(copy[i], 0);
	}

	testcase_end();
}

void test_write_v_after_read_g(void)
{
	testcase_begin(__func__);

	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(vec[i], 0);
	}

	// Write with VX
	vec = vec_splat_u8(1);

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		test_equal(vec[i], 1);
	}

	testcase_end();
}

int32_t start(void)
{
	test_init();
	test_read_g_after_write_v();
	test_read_v_after_write_g();
	test_write_g_after_read_v();
	test_write_v_after_read_g();
	test_summary();
	test_shutdown();

	return 0;
}
