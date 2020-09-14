#include "libnux/scheduling/TimerOneshot.hpp"
#include "libnux/unittest.h"

void test_timer_oneshot_next_event()
{
	libnux_testcase_begin("test_timer_oneshot_wants_run");
	auto timer = TimerOneshot();
	Event e;
	time_type runtime = 10;
	time_type window = 5;
	timer.set_deadline(runtime);
	timer.set_window(window);
	bool before = timer.next_event(e, 0);
	libnux_test_equal(before, false);
	bool in_window = timer.next_event(e, runtime - window);
	libnux_test_equal(in_window, true);
	libnux_test_equal(e.deadline, runtime);

	timer.set_deadline(runtime);
	bool at = timer.next_event(e, runtime);
	libnux_test_equal(at, true);
	libnux_test_equal(e.deadline, runtime);

	bool at_again = timer.next_event(e, runtime);
	libnux_test_equal(at_again, false);

	// reset state
	timer.set_deadline(runtime);
	bool after = timer.next_event(e, runtime + 1);
	libnux_test_equal(after, true);
	libnux_test_equal(e.deadline, runtime);

	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_timer_oneshot_next_event();
	libnux_test_summary();
	libnux_test_shutdown();
}
