#include "libnux/vx/attrib.h"
#include "libnux/vx/helper.h"
#include "libnux/vx/time.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

template <size_t N>
void test_sleep_cycles(char const* msg, times<N>& t, time_base_t sleep_time, time_base_t max_time)
{
	constexpr int inner_loop_count = 100;
	time_base_t post_loop;

	for (auto& pair : t) {
		pair.first = now();

		/* Run the critical section multiple times to reduce the influence of
		   start/stop overhead. */
		for (int i = 0; i <= inner_loop_count; i++) {
			sleep_cycles(sleep_time);
		}

		post_loop = now();
		pair.second = pair.first + (post_loop - pair.first) / inner_loop_count;
	}
	print_measurement(msg, t);
	test_true(min(t) > sleep_time);
	test_true(max(t) < max_time);
}

template <size_t N>
void measure_int_multiply(char const* msg, times<N>& t)
{
	int a = 10;
	int b = 20;
	ATTRIB_UNUSED int c;
	for (auto& pair : t) {
		pair.first = now();
		c = a * b;
		pair.second = now();
	}
	print_measurement(msg, t);
}

template <size_t N>
void measure_int_multiply_not_optimized_away(char const* msg, times<N>& t)
{
	int a = 10;
	int b = 20;
	ATTRIB_UNUSED int c{};
	do_not_optimize_away(a);
	do_not_optimize_away(b);
	do_not_optimize_away(c);
	for (auto& pair : t) {
		pair.first = now();
		c = a * b;
		pair.second = now();
	}
	print_measurement(msg, t);
}

int start()
{
	test_init();

	print_measurements_header();
	time_pair global;
	global.first = now();

	times<10> tentimes;
	times<1> onetime;
	/* Empty measurement using example measurement function. */
	testcase_begin("empty");
	measure_empty(tentimes);
	test_true(min(tentimes) > 0);
	testcase_end();

	/* Measure sleep_cycles with different cycles. */
	testcase_begin("sleep_cycles");
	// Disabled due to Issue: #4013
	// test_sleep_cycles("sleep_cycles 10", tentimes, 10, 600);
	test_sleep_cycles("sleep_cycles 100", onetime, 100, 600);
	test_sleep_cycles("sleep_cycles 1000", onetime, 1000, 1600);
	testcase_end();

	testcase_begin("optimization");
	/* Measure multiplication of two integer literals
	   without prohibiting precalculation. */
	measure_int_multiply("int operator*", tentimes);
	auto min_optimized_away = min(tentimes);
	/* Measure multiplication of two integer literals
	   and have them not optimized away, i.e. precalculated. */
	measure_int_multiply_not_optimized_away("int operator* not optimized away", tentimes);
	auto min_not_optimized_away = min(tentimes);
	test_true(min_optimized_away < min_not_optimized_away);
	testcase_end();

	global.second = now();
	print_measurement("All measurements with evaluation time", global);
	print_measurements_footer();

	test_summary();
	test_shutdown();
	return 0;
}
