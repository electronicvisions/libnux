#include "libnux/scheduling/TimerOneshot.hpp"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void test_timer_oneshot_next_event()
{
	testcase_begin("test_timer_oneshot_wants_run");
	auto timer = TimerOneshot();
	Event e;
	time_type runtime = 10;
	time_type window = 5;
	timer.set_deadline(runtime);
	timer.set_window(window);
	bool before = timer.next_event(e, 0);
	test_equal(before, false);
	bool in_window = timer.next_event(e, runtime - window);
	test_equal(in_window, true);
	test_equal(e.deadline, runtime);

	timer.set_deadline(runtime);
	bool at = timer.next_event(e, runtime);
	test_equal(at, true);
	test_equal(e.deadline, runtime);

	bool at_again = timer.next_event(e, runtime);
	test_equal(at_again, false);

	// reset state
	timer.set_deadline(runtime);
	bool after = timer.next_event(e, runtime + 1);
	test_equal(after, true);
	test_equal(e.deadline, runtime);

	testcase_end();
}

void start()
{
	test_init();
	test_timer_oneshot_next_event();
	test_summary();
	test_shutdown();
}
