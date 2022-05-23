#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void basic_test(void) {
	testcase_begin("basic_checks");

	char* s = reinterpret_cast<char*>(0);
	int a = 0;
	int b = 0;
	int c = 0;

	test_equal(0, a);
	test_equal(0, b);
	test_equal(0, c);

	test_equal(0, 1-1);
	test_equal(0, -1+1);
	test_equal(-1, -2+1);

	test_null(s);
	test_true(1);

	testcase_end();
}

void failing_test(void) {
	testcase_begin("failing_test");

	test_equal(0, 1+1);
	test_equal(0, 1+2);

	testcase_end();
}

void start(void) {
	test_init();
	basic_test();
	failing_test();
	test_summary();
	test_shutdown();
}
