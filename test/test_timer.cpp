#include "libnux/scheduling/Timer.hpp"
#include "libnux/unittest.h"

void test_timer_next_event()
{
	libnux_testcase_begin("timer.next_event");
	auto timer = Timer();
	Event e;
	time_t start = 1;
	time_t repeat = 2;
	time_t stop = 2;
	timer.set_first_deadline(start);
	timer.set_period(repeat);
	timer.set_num_periods(stop);

	bool too_early = timer.next_event(e, 0);
	libnux_test_equal(too_early, false);

	bool first = timer.next_event(e, start);
	libnux_test_equal(first, true);
	libnux_test_equal(e.deadline, start + repeat);

	bool already_run = timer.next_event(e, 2);
	libnux_test_equal(already_run, false);

	bool new_loop = timer.next_event(e, start + repeat);
	libnux_test_equal(new_loop, true);
	libnux_test_equal(e.deadline, start + 2 * repeat);

	bool too_late = timer.next_event(e, stop * repeat + start);
	libnux_test_equal(too_late, false);

	libnux_testcase_end();
}

void test_timer_miss_run()
{
	libnux_testcase_begin("timer.missed_count");
	auto timer = Timer();
	time_t start = 1;
	time_t repeat = 2;
	time_t stop = 4;
	timer.set_first_deadline(start);
	timer.set_period(repeat);
	timer.set_num_periods(stop);
	Event e;

	bool want_even_if_missed = timer.next_event(e, start + repeat * 2);
	libnux_test_equal(want_even_if_missed, true);
	libnux_test_equal(timer.get_missed_count(), 1);

	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_timer_next_event();
	test_timer_miss_run();
	libnux_test_summary();
	libnux_test_shutdown();
}
