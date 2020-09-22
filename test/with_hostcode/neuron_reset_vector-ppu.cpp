#include "libnux/reset_neurons.h"
#include "libnux/time.h"

int start()
{
	libnux::reset_neurons();

	sleep_cycles(USEC * 10);

	__vector uint8_t even = vec_splat_u8(1);
	__vector uint8_t odd = vec_splat_u8(0);
	libnux::reset_neurons(even, odd);

	return 0;
}
