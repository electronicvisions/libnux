#include "libnux/vx/unittest.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/malloc.h"

using namespace libnux::vx;

void start(void) {
	test_init();

	testcase_begin("test malloc");
	int* i = reinterpret_cast<int*>(malloc(sizeof(int)));
	int* j = reinterpret_cast<int*>(malloc(sizeof(int)));
	test_not_null(i);
	test_not_null(j);
	testcase_end();

	test_summary();
	test_shutdown();
}
