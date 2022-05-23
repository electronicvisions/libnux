#pragma once

#include "libnux/vx/attrib.h"

namespace libnux::vx {

ATTRIB_UNUSED static void sync()
{
	asm volatile("sync");
}

} // namespace libnux::vx
