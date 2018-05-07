#include "libnux/scheduling/Queue.hpp"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void test_push_and_pop_queue()
{
	testcase_begin("push_and_pop_queue");
	Queue<1, int> queue;
	int in = 1;
	queue.push(in);
	int out = 0;
	bool got_element = queue.pop(out);
	test_equal(in, out);
	test_equal(got_element, true);
	got_element = queue.pop(out);
	test_equal(got_element, false);
	testcase_end();
}

void test_overfill_queue()
{
	testcase_begin("overfill_queue");
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
	test_equal(got_element[0], true);
	test_equal(got_element[1], true);
	test_equal(got_element[2], false);
	// circular buffer loses first element
	test_equal(in[1], out[0]);
	test_equal(in[2], out[1]);
	testcase_end();
}

void start()
{
	test_init();
	test_push_and_pop_queue();
	test_overfill_queue();
	test_summary();
	test_shutdown();
}
