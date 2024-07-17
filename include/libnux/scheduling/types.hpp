#pragma once
#include "libnux/scheduling/Service.hpp"
#include "libnux/vx/spr.h"
#include <stdint.h>

#ifdef LIBNUX_TIME64
typedef uint64_t time_type;
#else
typedef uint32_t time_type;
#endif

inline time_type get_time()
{
	return static_cast<time_type>(libnux::vx::get_time_base() >> LIBNUX_TIME_RESOLUTION_SHIFT);
}

struct Event
{
	Service const* service;
	time_type deadline;
};

// scheduler signals
enum scheduler_signal
{
	// don't do anything except staying alive
	scheduler_wait = 0,
	// run, i.e. fetch events and execute them
	scheduler_run = 1,
	// execute events in queue, but don't fetch new ones
	scheduler_finish = 2,
	// exit execution function
	scheduler_exit = 3,
};
