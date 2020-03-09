#include <s2pp.h>
#include <libnux/unittest.h>
#include <libnux/mailbox.h>

void test_vector_add() {
	libnux_testcase_begin("vector_add");

	__vector uint8_t lhs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	__vector uint8_t rhs = vec_splat_u8(1);
	__vector uint8_t res = vec_add(lhs, rhs);
	for (uint32_t index = 0; index < 16; index++) {
		libnux_test_equal(res[index], index + 1);
	}

	libnux_testcase_end();
}

void test_vector_addbfs() {
	libnux_testcase_begin("vector_addbfs");

	libnux_mailbox_write_string("Non saturating addition\n");
	{
		__vector int8_t lhs = vec_splat_s8(0);
		__vector int8_t rhs = vec_splat_s8(1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < 16; index++) {
			libnux_test_equal(lhs[index], (int8_t)(0));
			libnux_test_equal(rhs[index], (int8_t)(1));
			libnux_test_equal(res[index], (int8_t)(1));
		}
	}
	libnux_mailbox_write_string("Saturating addition\n");
	{
		__vector int8_t lhs = vec_splat_s8(127);
		__vector int8_t rhs = vec_splat_s8(1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < 16; index++) {
			libnux_test_equal(res[index], (int8_t)(127));
		}
	}
	libnux_mailbox_write_string("Non saturating addition with signed\n");
	{
		__vector int8_t lhs = vec_splat_s8(2);
		__vector int8_t rhs = vec_splat_s8(-1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < 16; index++) {
			libnux_test_equal(res[index], (int8_t)(1));
		}
	}
	libnux_mailbox_write_string("Saturating addition with signed\n");
	{
		__vector int8_t lhs = vec_splat_s8(-128);
		__vector int8_t rhs = vec_splat_s8(-1);
		__vector int8_t res = fxv_addbfs_c(lhs, rhs, 0);
		for (uint32_t index = 0; index < 16; index++) {
			libnux_test_equal(res[index], (int8_t)(-128));
		}
	}

	libnux_testcase_end();
}

void test_ptr_add() {
	libnux_testcase_begin("ptr_add");

	// Checks if the pointer to a vector is shifted by 16 bytes at an addition
	// of 1.
	__vector uint8_t* base = (__vector uint8_t*) (0x0);
	__vector uint8_t* shifted = base + 1;
	libnux_test_equal(shifted, (__vector uint8_t*) (0x10));

	libnux_testcase_end();
}

void start(void) {
	libnux_test_init();
	test_ptr_add();
	test_vector_add();
	test_vector_addbfs();
	libnux_test_summary();
	libnux_test_shutdown();
}
