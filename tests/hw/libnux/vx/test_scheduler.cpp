#include "libnux/scheduling.hpp"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

int test_run_var = 0;
void test_run()
{
	test_run_var = 1;
}
int test_no_run_var = 0;
void test_no_run()
{
	test_no_run_var = 1;
}
// test run_service
// verify, that only service with matching event.id is run
void test_scheduler_run_service()
{
	testcase_begin("test_scheduler_run_service");
	Service service = &test_run;
	Timer timer(service);
	timer.set_period(10);
	timer.set_num_periods(2);
	auto event_sources = std::tie(timer);

	auto scheduler = Scheduler<1>();

	scheduler.fetch_events_timed(event_sources, 5);
	scheduler.run_queue_service();
	test_equal(test_run_var, 1);
	test_equal(test_no_run_var, 0);
	testcase_end();
}

uint8_t test_var = 0;
template <uint8_t i>
void test()
{
	if (test_var == i) {
		++test_var;
	}
}

class source
{
	Service service;
	time_base_t deadline;
	bool has_been_fetched;

public:
	source(Service srv, time_base_t d)
	{
		service = srv;
		deadline = d;
		has_been_fetched = false;
	}
	bool next_event(Event& e, __attribute__((unused)) time_base_t t)
	{
		if (!has_been_fetched) {
			e.service = &service;
			e.deadline = deadline;
			has_been_fetched = true;
			return true;
		}
		return false;
	}
};

// verify sort_earliest_first
// sorts events such that highest
// priority event is run first
void test_scheduler_sort_queue()
{
	testcase_begin("test_scheduler_sort_queue");
	auto scheduler = Scheduler<3>();

	Service service0 = &test<0>;
	Service service1 = &test<1>;
	Service service2 = &test<2>;
	source a(service0, 0);
	source b(service1, 1);
	source c(service2, 2);

	auto event_sources = std::tie(a, b, c);

	scheduler.fetch_events_timed(event_sources, 16);
	test_var = 0;
	for (int i = 0; i < 3; ++i) {
		scheduler.sort_earliest_first();
		scheduler.run_queue_service();
	}
	test_equal(test_var, 3);

	source a2(service0, 2);
	source b2(service1, 1);
	source c2(service2, 0);
	auto event_sources2 = std::tie(a2, b2, c2);
	scheduler.fetch_events_timed(event_sources2, 16);
	test_var = 0;
	for (int i = 0; i < 3; ++i) {
		scheduler.sort_earliest_first();
		scheduler.run_queue_service();
	}
	// only service0 works, services1,2 don't alter value
	// as they're run before service0
	test_equal(test_var, 1);

	testcase_end();
}

void start()
{
	test_init();
	test_scheduler_run_service();
	test_scheduler_sort_queue();
	test_summary();
	test_shutdown();
}
