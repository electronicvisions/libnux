#pragma once

#include <stddef.h>
#include <stdint.h>

#include "libnux/vx/attrib.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/spr.h"
#include "libnux/vx/time.h"

namespace libnux::vx {

constexpr static uint32_t default_ppu_cycles_per_us = 250;

void sleep_cycles(uint32_t cycles);

/*
	Get current absolute time in ppu cycles.
	Force get_time_base() not to be inlined
	because otherwise, mfsprs might get grouped together,
	rendering measurement userless.
*/
time_base_t now();

/*
	Pair of time values.
*/
struct time_pair
{
	time_base_t first;
	time_base_t second;
};

/*
	Times is holding N time pairs and features convenience iterators.
*/
template <size_t N>
class times
{
	time_pair values[N];
public:
	time_pair& operator[](size_t i)
	{
		return values[i];
	}
	time_pair const& operator[](size_t i) const
	{
		return values[i];
	}

	time_pair* begin()
	{
		return &values[0];
	}
	time_pair* end()
	{
		return &values[N];
	}
	time_pair const* begin() const
	{
		return &values[0];
	}
	time_pair const* end() const
	{
		return &values[N];
	}
};

/*
	Average times structure time differences.
*/
template <size_t N>
time_base_t average(times<N> const& t)
{
	time_base_t avg = 0;
	for (auto pair: t) {
		avg += pair.second - pair.first;
	}
	avg /= N;
	return avg;
}

/*
	Find maximum in time measurements.
*/
template <size_t N>
time_base_t max(times<N> const& t)
{
	time_base_t tmp;
	time_base_t t_max = 0;
	for (auto pair: t) {
		tmp = pair.second - pair.first;
		if (t_max < tmp) {
			t_max = tmp;
		}
	}
	return t_max;
}

/*
	Find minimum in time measurements.
*/
template <size_t N>
time_base_t min(times<N> const& t)
{
	time_base_t tmp;
	time_base_t t_min = t.begin()->second - t.begin()->first;
	for (auto pair: t) {
		tmp = pair.second - pair.first;
		if (t_min > tmp) {
			t_min = tmp;
		}
	}
	return t_min;
}

/*
	Print measurement results.
*/
template <size_t N>
void print_measurement(char const* msg, times<N> const& t)
{
	mailbox_write_string(msg);
	mailbox_write_string(": ");

	time_base_t t_avg = average(t);
	time_base_t t_max = max(t);
	time_base_t t_min = min(t);
	mailbox_write_int(t_avg);
	mailbox_write_string(" (max: ");
	mailbox_write_int(t_max);
	mailbox_write_string(", min: ");
	mailbox_write_int(t_min);
	mailbox_write_string(")\n");
}

/*
	Print measurement results.
*/
void print_measurement(char const* msg, time_pair const& t);

/*
	Print measurements header.
*/
void print_measurements_header();

/*
	Print measurements footer.
*/
void print_measurements_footer();

/*
	Time measurement example, measuring empty event.
	It thus can be included in other measurements
	with possible customized optimization
	to measure the empty measurement offset.
	The measurement is carried out N times.
*/
template <size_t N>
void measure_empty(times<N>& t)
{
	for (auto& pair: t) {
		pair.first = now();
		// event to be measured goes here
		pair.second = now();
	}
	print_measurement("empty", t);
}

} // namespace libnux::vx
