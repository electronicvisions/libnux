#include <s2pp.h>
#include "libnux/dls.h"
#include "libnux/unittest.h"

void test_read_g_after_write_v(void)
{
	libnux_testcase_begin(__func__);

	// Write with VX
	__vector uint8_t vec = vec_splat_u8(0xff);

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		libnux_test_equal(vec[i], 0xff);
	}

	libnux_testcase_end();
}

void test_read_v_after_write_g(void)
{
	libnux_testcase_begin(__func__);

#ifndef LIBNUX_DLS_VERSION_VX
	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

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
#ifndef LIBNUX_DLS_VERSION_VX
		: [temp] "=&kv" (temp),
#else
		: [temp] "=&qv" (temp),
#endif
		  [tgt_ptr] "=Z"(copy)
		: [src_ptr] "Z"(vec)
	);
	// clang-format on

	// Check the copy
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		libnux_test_equal(copy[i], 1);
	}

	libnux_testcase_end();
}

void test_write_g_after_read_v(void)
{
	libnux_testcase_begin(__func__);

#ifndef LIBNUX_DLS_VERSION_VX
	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
	__vector uint8_t vec = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
	__vector uint8_t copy;

	// Read with VX
	// clang-format off
	asm volatile(
		// Read a correlation measurement which takes ~300 cycles
		"fxvinx %[copy], %[dls_causal_base], %[index]\n"
		// Load the vector from memory into a register
		"fxvlax %[copy], %y[ptr]\n"
		"sync"
#ifndef LIBNUX_DLS_VERSION_VX
		: [copy] "=kv" (copy)
#else
		: [copy] "=qv" (copy)
#endif
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
		libnux_test_equal(copy[i], 0);
	}

	libnux_testcase_end();
}

void test_write_v_after_read_g(void)
{
	libnux_testcase_begin(__func__);

#ifndef LIBNUX_DLS_VERSION_VX
	__vector uint8_t vec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
	__vector uint8_t vec = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
		libnux_test_equal(vec[i], 0);
	}

	// Write with VX
	vec = vec_splat_u8(1);

	// Read with GPP
	for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
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
