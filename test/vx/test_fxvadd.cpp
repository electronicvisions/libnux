#include <s2pp.h>
#include "libnux/vx/mailbox.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void test_fxvadd()
{
	testcase_begin(__func__);
	/* Addition should be modulo arithmetic by default */
	{
		__vector int8_t vec = fxv_splatb(0xff);
		__vector int8_t const ones = fxv_splatb(1);
		vec = fxv_add(vec, ones);
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			test_equal(vec[i], 0);
		}
	}
	testcase_end();
}

void test_fxvaddfs()
{
	testcase_begin(__func__);
	/* Check if saturating arithmetic works */
	{
		__vector int8_t vec = fxv_splatb(0x7f);
		__vector int8_t const ones = fxv_splatb(1);
		vec = fxv_addfs(vec, ones);
		for (uint32_t i = 0; i < sizeof(__vector uint8_t); i++) {
			test_equal(vec[i], 0x7f);
		}
	}
	testcase_end();
}

void start(void)
{
	test_init();
	test_fxvadd();
	test_fxvaddfs();
	test_summary();
	test_shutdown();
}
