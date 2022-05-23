#pragma once

#include "libnux/vx/time.h"

namespace libnux::vx::v3 {

constexpr static uint32_t default_ppu_cycles_per_us =
   libnux::vx::default_ppu_cycles_per_us;

using libnux::vx::sleep_cycles;

using libnux::vx::now;

using time_pair = libnux::vx::time_pair;

template <size_t N>
using times = libnux::vx::times<N>;

using libnux::vx::average;
using libnux::vx::max;
using libnux::vx::min;

using libnux::vx::print_measurement;
using libnux::vx::print_measurements_header;
using libnux::vx::print_measurements_footer;
using libnux::vx::measure_empty;

} // namespace libnux::vx::v3
