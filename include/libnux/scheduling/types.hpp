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
