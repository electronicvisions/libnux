#include <s2pp.h>
#include "libnux/vx/unittest.h"
#include "libnux/vx/mailbox.h"

using namespace libnux::vx;

void test_fxvsel_inline_asm() {
	testcase_begin(__func__);
	// Create a vector with 0, +1, -1, 0, +1, -1, ... . The -1 is explicitely
	// initialized as its two's complement, which is 0xff
	__vector uint8_t cmp = {
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff,
		0x00, 0x01, 0x00, 0xff, 0x00, 0x01, 0x00, 0xff};
	__vector uint8_t lhs = vec_splat_u8(2);
	__vector uint8_t rhs = vec_splat_u8(1);
	/* Condition always */
	{
		__vector uint8_t res;
		asm volatile (
				// Use vector cmp for component wise comparison
				"fxvcmpb %[cmp]\n"
				// Select third argument if condition "always" matches, e.g.
				// always select rhs
				"fxvsel 1, %[lhs], %[rhs], 0\n"
				// Store at the memory location of vector res
				"fxvstax 1, %[zero], %[res_ptr]\n"
				// Call sync to wait for store operation to finish
				"sync"
				: /* no output */
				: [cmp] "qv" (cmp),
				  [lhs] "qv" (lhs),
				  [rhs] "qv" (rhs),
				  [res_ptr] "r" (&res),
				  [zero] "r" (0)
				: "qv1", "memory");
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			test_equal(res[i], rhs[i]);
		}
	}
	/* Condition gt */
	{
		__vector uint8_t res;
		asm volatile (
				"fxvcmpb %[cmp]\n"
				"fxvsel 1, %[lhs], %[rhs], 1\n"
				"fxvstax 1, %[zero], %[res_ptr]\n"
				"sync"
				: /* no output */
				: [cmp] "qv" (cmp),
				  [lhs] "qv" (lhs),
				  [rhs] "qv" (rhs),
				  [res_ptr] "r" (&res),
				  [zero] "r" (0)
				: "qv1", "memory");
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			if ((int8_t)(cmp[i]) > 0) {
				test_equal(res[i], rhs[i]);
			} else {
				test_equal(res[i], lhs[i]);
			}
		}
	}
	/* Condition lt */
	{
		__vector uint8_t res;
		asm volatile (
				"fxvcmpb %[cmp]\n"
				"fxvsel 1, %[lhs], %[rhs], 2\n"
				"fxvstax 1, %[zero], %[res_ptr]\n"
				"sync"
				: /* no output */
				: [cmp] "qv" (cmp),
				  [lhs] "qv" (lhs),
				  [rhs] "qv" (rhs),
				  [res_ptr] "r" (&res),
				  [zero] "r" (0)
				: "qv1", "memory");
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			if ((int8_t)(cmp[i]) < 0) {
				test_equal(res[i], rhs[i]);
			} else {
				test_equal(res[i], lhs[i]);
			}
		}
	}
	/* Condition eq */
	{
		__vector uint8_t res;
		asm volatile (
				"fxvcmpb %[cmp]\n"
				"fxvsel 1, %[lhs], %[rhs], 3\n"
				"fxvstax 1, %[zero], %[res_ptr]\n"
				"sync"
				: /* no output */
				: [cmp] "qv" (cmp),
				  [lhs] "qv" (lhs),
				  [rhs] "qv" (rhs),
				  [res_ptr] "r" (&res),
				  [zero] "r" (0)
				: "qv1", "memory");
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			if ((int8_t)(cmp[i]) == 0) {
				test_equal(res[i], rhs[i]);
			} else {
				test_equal(res[i], lhs[i]);
			}
		}
	}

	testcase_end();
}

void start(void) {
	test_init();
	test_fxvsel_inline_asm();
	test_summary();
	test_shutdown();
}
