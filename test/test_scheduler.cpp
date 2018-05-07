#include "libnux/scheduling.hpp"
#include "libnux/unittest.h"

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
	libnux_testcase_begin("test_scheduler_run_service");
	auto service = Service_Function<0, test_run>();
	auto service_no_run = Service_Function<1, test_no_run>();
	auto services = std::tie(service, service_no_run);
	Timer timer;
	timer.set_service(service);
	timer.set_period(10);
	timer.set_num_periods(2);
	auto event_sources = std::tie(timer);

	auto scheduler = Scheduler<1>();

	scheduler.fetch_events_timed(event_sources, 5);
	scheduler.run_queue_service(services);
	libnux_test_equal(test_run_var, 1);
	libnux_test_equal(test_no_run_var, 0);
	libnux_testcase_end();
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
	service_id id;
	time_base_t deadline;
	bool has_been_fetched;

public:
	source(service_id i, time_base_t d)
	{
		id = i;
		deadline = d;
		has_been_fetched = false;
	}
	bool next_event(Event& e, __attribute__((unused)) time_base_t t)
	{
		if (!has_been_fetched) {
			e.id = id;
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
	libnux_testcase_begin("test_scheduler_sort_queue");
	auto scheduler = Scheduler<3>();
	source a(0, 0);
	source b(1, 1);
	source c(2, 2);

	auto event_sources = std::tie(a, b, c);
	auto service0 = Service_Function<0, test<0> >();
	auto service1 = Service_Function<1, test<1> >();
	auto service2 = Service_Function<2, test<2> >();
	auto services = std::tie(service0, service1, service2);

	scheduler.fetch_events_timed(event_sources, 16);
	test_var = 0;
	for (int i = 0; i < 3; ++i) {
		scheduler.sort_earliest_first();
		scheduler.run_queue_service(services);
	}
	libnux_test_equal(test_var, 3);

	source a2(0, 2);
	source b2(1, 1);
	source c2(2, 0);
	auto event_sources2 = std::tie(a2, b2, c2);
	scheduler.fetch_events_timed(event_sources2, 16);
	test_var = 0;
	for (int i = 0; i < 3; ++i) {
		scheduler.sort_earliest_first();
		scheduler.run_queue_service(services);
	}
	// only service0 works, services1,2 don't alter value
	// as they're run before service0
	libnux_test_equal(test_var, 1);

	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_scheduler_run_service();
	test_scheduler_sort_queue();
	libnux_test_summary();
	libnux_test_shutdown();
}
