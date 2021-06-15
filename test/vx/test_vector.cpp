#include <s2pp.h>
#include "libnux/vx/unittest.h"
#include "libnux/vx/mailbox.h"

using namespace libnux::vx;

void test_vector_add() {
	testcase_begin("vector_add");

	__vector uint8_t lhs = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
		44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
		58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
		72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
		86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
		100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
		124, 125, 126, 127};
	__vector uint8_t rhs = vec_splat_u8(1);
	__vector uint8_t res = vec_add(lhs, rhs);
	for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
		test_equal(res[index], index + 1);
	}

	testcase_end();
}

void test_vector_addbfs() {
	testcase_begin("vector_addbfs");

	mailbox_write_string("Non saturating addition\n");
	{
		__vector int8_t lhs = vec_splat_s8(0);
		__vector int8_t rhs = vec_splat_s8(1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(lhs[index], (int8_t)(0));
			test_equal(rhs[index], (int8_t)(1));
			test_equal(res[index], (int8_t)(1));
		}
	}
	mailbox_write_string("Saturating addition\n");
	{
		__vector int8_t lhs = vec_splat_s8(127);
		__vector int8_t rhs = vec_splat_s8(1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], (int8_t)(127));
		}
	}
	mailbox_write_string("Non saturating addition with signed\n");
	{
		__vector int8_t lhs = vec_splat_s8(2);
		__vector int8_t rhs = vec_splat_s8(-1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], (int8_t)(1));
		}
	}
	mailbox_write_string("Saturating addition with signed\n");
	{
		__vector int8_t lhs = vec_splat_s8(-128);
		__vector int8_t rhs = vec_splat_s8(-1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < sizeof(__vector uint8_t); index++) {
			test_equal(res[index], (int8_t)(-128));
		}
	}

	testcase_end();
}

void test_ptr_add() {
	testcase_begin("ptr_add");

	// Checks if the pointer to a vector is shifted by the size of the vector in
	// bytes at an addition of 1.
	__vector uint8_t* base = (__vector uint8_t*) (0x0);
	__vector uint8_t* shifted = base + 1;
	test_equal(shifted, (__vector uint8_t*) (sizeof(__vector uint8_t)));

	testcase_end();
}

void start(void) {
	test_init();
	test_ptr_add();
	test_vector_add();
	test_vector_addbfs();
	test_summary();
	test_shutdown();
}
