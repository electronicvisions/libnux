#include "libnux/scheduling/Queue.hpp"
#include "libnux/unittest.h"

void test_push_and_pop_queue()
{
	libnux_testcase_begin("push_and_pop_queue");
	Queue<1, int> queue;
	int in = 1;
	queue.push(in);
	int out = 0;
	bool got_element = queue.pop(out);
	libnux_test_equal(in, out);
	libnux_test_equal(got_element, true);
	got_element = queue.pop(out);
	libnux_test_equal(got_element, false);
	libnux_testcase_end();
}

void test_overfill_queue()
{
	libnux_testcase_begin("overfill_queue");
	Queue<2, int> queue;
	int in[3] = {1, 2, 3};
	int out[3] = {0, 0, 0};
	for (int i = 0; i < 3; ++i) {
		queue.push(in[i]);
	}
	bool got_element[3];
	for (int i = 0; i < 3; ++i) {
		got_element[i] = queue.pop(out[i]);
	}
	// no third element to be fetched
	libnux_test_equal(got_element[0], true);
	libnux_test_equal(got_element[1], true);
	libnux_test_equal(got_element[2], false);
	// circular buffer loses first element
	libnux_test_equal(in[1], out[0]);
	libnux_test_equal(in[2], out[1]);
	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_push_and_pop_queue();
	test_overfill_queue();
	libnux_test_summary();
	libnux_test_shutdown();
}
