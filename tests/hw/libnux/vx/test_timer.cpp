#include "libnux/scheduling/Timer.hpp"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void foo() {}

void test_timer_next_event()
{
	testcase_begin("timer.next_event");
	auto timer = Timer(&foo);
	Event e;
	time_type start = 1;
	time_type repeat = 2;
	time_type stop = 2;
	timer.set_first_deadline(start);
	timer.set_period(repeat);
	timer.set_num_periods(stop);

	bool too_early = timer.next_event(e, 0);
	test_equal(too_early, false);

	bool first = timer.next_event(e, start);
	test_equal(first, true);
	test_equal(e.deadline, start + repeat);

	bool already_run = timer.next_event(e, 2);
	test_equal(already_run, false);

	bool new_loop = timer.next_event(e, start + repeat);
	test_equal(new_loop, true);
	test_equal(e.deadline, start + 2 * repeat);

	bool too_late = timer.next_event(e, stop * repeat + start);
	test_equal(too_late, false);

	testcase_end();
}

void test_timer_miss_run()
{
	testcase_begin("timer.missed_count");
	auto timer = Timer(&foo);
	time_type start = 1;
	time_type repeat = 2;
	time_type stop = 4;
	timer.set_first_deadline(start);
	timer.set_period(repeat);
	timer.set_num_periods(stop);
	Event e;

	bool want_even_if_missed = timer.next_event(e, start + repeat * 2);
	test_equal(want_even_if_missed, true);
	test_equal(timer.get_missed_count(), 1ul);

	testcase_end();
}

void start()
{
	test_init();
	test_timer_next_event();
	test_timer_miss_run();
	test_summary();
	test_shutdown();
}
