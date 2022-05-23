#include "libnux/vx/unittest.h"
#include <s2pp.h>
#include <cstddef>

using namespace libnux::vx;

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
	testcase_begin("size == 1B");
	test_equal(sizeof(bool), static_cast<size_t>(1));
	testcase_end();
}

void test_not()
{
	testcase_begin("not");
	bool e1 = !true;
	bool e2 = !false;
	bool e3 = !!false;
	bool e4 = !!true;
	test_equal(e1, false);
	test_equal(e2, true);
	test_equal(e3, false);
	test_equal(e4, true);
	testcase_end();
}

void test_and()
{
	testcase_begin("and");
	bool e1 = true && false;
	bool e2 = true && true;
	bool e3 = false && false;
	test_equal(e1, false);
	test_equal(e2, true);
	test_equal(e3, false);
	testcase_end();
}

void test_or()
{
	testcase_begin("or");
	bool e1 = true || false;
	bool e2 = true || true;
	bool e3 = false || false;
	test_equal(e1, true);
	test_equal(e2, true);
	test_equal(e3, false);
	testcase_end();
}

void start()
{
	test_init();
	test_size();
	test_not();
	test_and();
	test_or();
	test_summary();
	test_shutdown();
}
