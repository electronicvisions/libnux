#include "libnux/time.h"
#include "libnux/spr.h"

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
	libnux_mailbox_write_string(msg);
	libnux_mailbox_write_string(": ");
	libnux_mailbox_write_int(t.second - t.first);
	libnux_mailbox_write_string("\n");
}

void print_measurements_header()
{
	libnux_mailbox_write_string("begin time measurements [ppu cycles]\n\n");
}

void print_measurements_footer()
{
	libnux_mailbox_write_string("\nstop time measurements [ppu cycles]");
}
