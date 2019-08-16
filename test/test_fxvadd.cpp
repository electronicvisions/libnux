#include <s2pp.h>
#include "libnux/mailbox.h"
#include "libnux/unittest.h"

void test_fxvadd()
{
	libnux_testcase_begin(__func__);
	/* Addition should be modulo arithmetic by default */
	{
		vector int8_t vec = fxv_splatb(0xff);
		vector int8_t const ones = fxv_splatb(1);
		vec = fxv_add(vec, ones);
		for (uint32_t i = 0; i < sizeof(vector uint8_t); i++) {
			libnux_test_equal(vec[i], 0);
		}
	}
	libnux_testcase_end();
}

void test_fxvaddfs()
{
	libnux_testcase_begin(__func__);
	/* Check if saturating arithmetic works */
	{
		vector int8_t vec = fxv_splatb(0x7f);
		vector int8_t const ones = fxv_splatb(1);
		vec = fxv_addfs(vec, ones);
		for (uint32_t i = 0; i < sizeof(vector uint8_t); i++) {
			libnux_test_equal(vec[i], 0x7f);
		}
	}
	libnux_testcase_end();
}

void start(void)
{
	libnux_test_init();
	test_fxvadd();
	test_fxvaddfs();
	libnux_test_summary();
	libnux_test_shutdown();
}
