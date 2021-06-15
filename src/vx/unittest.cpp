#include "libnux/system.h"
#include "libnux/vx/unittest.h"

namespace libnux::vx {

static uint32_t num_passed = 0;
static uint32_t num_failed = 0;

static uint32_t num_testcases_passed= 0;
static uint32_t num_testcases_failed = 0;

static uint32_t testcase_passed_offset = 0;
static uint32_t testcase_failed_offset = 0;

static test_action_type test_action = test_action_warning;


void test_init(void) {
}

void test_shutdown(void) {
	test_write_termination();
	exit(num_testcases_failed != 0);
}

uint32_t test_get_passed(void) {
	return num_passed;
}

uint32_t test_get_failed(void) {
	return num_failed;
}

uint32_t testcase_get_passed(void) {
	return num_testcases_passed;
}

uint32_t testcase_get_failed(void) {
	return num_testcases_failed;
}

void test_set_action(test_action_type const action) {
	test_action = action;
}

test_action_type test_get_action(void) {
	return test_action;
}

void test_inc_passed(void) {
	num_passed++;
}

void test_inc_failed(void) {
	num_failed++;
}

uint32_t get_num_passed_in_testcase(void) {
	return num_passed - testcase_passed_offset;
}

uint32_t get_num_failed_in_testcase(void) {
	return num_failed - testcase_failed_offset;
}

void testcase_begin(char const * name) {
	test_write_string("[ Run      ] ");
	test_write_string(name);
	test_write_string("\n");
	testcase_passed_offset = num_passed;
	testcase_failed_offset = num_failed;
}

void testcase_end(void) {
	if(get_num_failed_in_testcase() == 0) {
		test_write_string("[       Ok ] Passed ");
		test_write_int(get_num_passed_in_testcase());
		test_write_string(" tests\n");
		num_testcases_passed++;
	} else {
		test_write_string("[  Failed  ]\n");
		num_testcases_failed++;
	}
}

void test_summary(void) {
	test_write_string("\n[==========]\n");
	test_write_string("[  Passed  ] ");
	test_write_int(num_testcases_passed);
	test_write_string(" test cases\n");
	test_write_string("[  Failed  ] ");
	test_write_int(num_testcases_failed);
	test_write_string(" test cases\n");

	test_write_string("\n[==========]\n");
	test_write_string("[  Passed  ] ");
	test_write_int(num_passed);
	test_write_string(" tests\n");
	test_write_string("[  Failed  ] ");
	test_write_int(num_failed);
	test_write_string(" tests\n");
}

} // namespace libnux::vx
