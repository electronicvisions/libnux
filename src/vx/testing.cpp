#include "libnux/system.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void TestFactory::register_test(TestRunFunc creator)
{
	if (id == max_num_tests) {
		mailbox_write_string("#tests > max_num_tests.\n");
		exit(1);
	}
	creators[id] = creator;
	id++;
}

int TestFactory::call_all()
{
	for (size_t i = 0; i < id; ++i) {
		(creators[i])();
	}
	test_summary();
	return testcase_get_failed() != 0;
}

size_t TestFactory::id = 0;
TestFactory::TestRunFunc TestFactory::creators[max_num_tests];

int RUN_ALL_TESTS()
{
	return TestFactory::call_all();
}
