#include "libnux/attrib.h"
extern "C" {
#include "libnux/unittest.h"
}
#include "libnux/time.hpp"
#include "libnux/helpers.hpp"

template <size_t N>
void test_sleep_cycles(char const* msg, times<N>& t, time_base_t sleep_time, time_base_t max_time)
{
	for (auto& pair : t) {
		pair.first = now();
		sleep_cycles(sleep_time);
		pair.second = now();
	}
	print_measurement(msg, t);
	libnux_test_true(min(t) > sleep_time);
	libnux_test_true(max(t) < max_time);
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
	ATTRIB_UNUSED int c;
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

extern "C" int start()
{
	libnux_test_init();

	print_measurements_header();
	time_pair global;
	global.first = now();

	times<10> t;
	/* Empty measurement using example measurement function. */
	libnux_testcase_begin("empty");
	measure_empty(t);
	libnux_test_true(min(t) > 0);
	libnux_testcase_end();

	/* Measure sleep_cycles with different cycles. */
	libnux_testcase_begin("sleep_cycles");
	test_sleep_cycles("sleep_cycles 10", t, 10, 400);
	test_sleep_cycles("sleep_cycles 100", t, 100, 400);
	test_sleep_cycles("sleep_cycles 1000", t, 1000, 1300);
	test_sleep_cycles("sleep_cycles 10000", t, 10000, 10300);
	test_sleep_cycles("sleep_cycles 100000", t, 100000, 100300);
	libnux_testcase_end();

	libnux_testcase_begin("optimization");
	/* Measure multiplication of two integer literals
	   without prohibiting precalculation. */
	measure_int_multiply("int operator*", t);
	auto min_optimized_away = min(t);
	/* Measure multiplication of two integer literals
	   and have them not optimized away, i.e. precalculated. */
	measure_int_multiply_not_optimized_away("int operator* not optimized away", t);
	auto min_not_optimized_away = min(t);
	libnux_test_true(min_optimized_away < min_not_optimized_away);
	libnux_testcase_end();

	global.second = now();
	print_measurement("All measurements with evaluation time", global);
	print_measurements_footer();

	libnux_test_summary();
	libnux_test_shutdown();
	return 0;
}
