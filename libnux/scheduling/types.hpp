#pragma once
#include <stdint.h>
#include "libnux/spr.h"

typedef uint8_t service_id;

#ifdef LIBNUX_TIME64
typedef uint64_t time_type;
#else
typedef uint32_t time_type;
#endif

time_type get_time()
{
	return static_cast<time_type>(get_time_base() >> LIBNUX_TIME_RESOLUTION_SHIFT);
}

struct Event
{
	service_id id = 0;
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
