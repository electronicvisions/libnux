#include "libnux/vx/time.h"
#include "libnux/vx/spr.h"

namespace libnux::vx {

/*
	Idle for (approx.) `cycles` cycles.
*/
void __attribute__((optimize("O2"))) sleep_cycles(uint32_t cycles)
{
	static const uint8_t offset = 9;
	time_base_t start;
	start = get_time_base();
	while ((uint32_t)(get_time_base() - start) <= (cycles - offset));
}

time_base_t now()
{
	return get_time_base();
}

void print_measurement(char const* msg, time_pair const& t)
{
	mailbox_write_string(msg);
	mailbox_write_string(": ");
	mailbox_write_int(t.second - t.first);
	mailbox_write_string("\n");
}

void print_measurements_header()
{
	mailbox_write_string("begin time measurements [ppu cycles]\n\n");
}

void print_measurements_footer()
{
	mailbox_write_string("\nstop time measurements [ppu cycles]");
}

} // namespace libnux::vx
