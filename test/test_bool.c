#include <s2pp.h>
#include "libnux/unittest.h"

// needed for current gcc-nux
#ifndef false
// clang-format off
#define false (bool) 0
// clang-format on
#endif
#ifndef true
#define true !false
#endif

void test_size()
{
	libnux_testcase_begin("size == 4B");
	libnux_test_equal(sizeof(bool), 4);
	libnux_testcase_end();
}

void test_not()
{
	libnux_testcase_begin("not");
	bool e1 = !true;
	bool e2 = !false;
	bool e3 = !!false;
	bool e4 = !!true;
	libnux_test_equal(e1, false);
	libnux_test_equal(e2, true);
	libnux_test_equal(e3, false);
	libnux_test_equal(e4, true);
	libnux_testcase_end();
}

void test_and()
{
	libnux_testcase_begin("and");
	bool e1 = true && false;
	bool e2 = true && true;
	bool e3 = false && false;
	libnux_test_equal(e1, false);
	libnux_test_equal(e2, true);
	libnux_test_equal(e3, false);
	libnux_testcase_end();
}

void test_or()
{
	libnux_testcase_begin("or");
	bool e1 = true || false;
	bool e2 = true || true;
	bool e3 = false || false;
	libnux_test_equal(e1, true);
	libnux_test_equal(e2, true);
	libnux_test_equal(e3, false);
	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_size();
	test_not();
	test_and();
	test_or();
	libnux_test_summary();
	libnux_test_shutdown();
}
