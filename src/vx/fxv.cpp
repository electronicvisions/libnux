#include "libnux/vx/fxv.h"
#include "libnux/vx/dls.h"
#include "libnux/vx/sync.h"
#include <stddef.h>
#include <stdint.h>

namespace libnux::vx {

void fxv_zero_vrf() {
	volatile uint8_t zeros[dls_bytes_per_vector];
	for (size_t i = 0; i < dls_bytes_per_vector; i++) {
		zeros[i] = 0;
	}
	sync();

	asm volatile(
		"fxvlax 0, 0, %[z]\n"
		"fxvlax 1, 0, %[z]\n"
		"fxvlax 2, 0, %[z]\n"
		"fxvlax 3, 0, %[z]\n"
		"fxvlax 4, 0, %[z]\n"
		"fxvlax 5, 0, %[z]\n"
		"fxvlax 6, 0, %[z]\n"
		"fxvlax 7, 0, %[z]\n"
		"fxvlax 8, 0, %[z]\n"
		"fxvlax 9, 0, %[z]\n"
		"fxvlax 10, 0, %[z]\n"
		"fxvlax 11, 0, %[z]\n"
		"fxvlax 12, 0, %[z]\n"
		"fxvlax 13, 0, %[z]\n"
		"fxvlax 14, 0, %[z]\n"
		"fxvlax 15, 0, %[z]\n"
		"fxvlax 16, 0, %[z]\n"
		"fxvlax 17, 0, %[z]\n"
		"fxvlax 18, 0, %[z]\n"
		"fxvlax 19, 0, %[z]\n"
		"fxvlax 20, 0, %[z]\n"
		"fxvlax 21, 0, %[z]\n"
		"fxvlax 22, 0, %[z]\n"
		"fxvlax 23, 0, %[z]\n"
		"fxvlax 24, 0, %[z]\n"
		"fxvlax 25, 0, %[z]\n"
		"fxvlax 26, 0, %[z]\n"
		"fxvlax 27, 0, %[z]\n"
		"fxvlax 28, 0, %[z]\n"
		"fxvlax 29, 0, %[z]\n"
		"fxvlax 30, 0, %[z]\n"
		"fxvlax 31, 0, %[z]\n"
		:	/* no output */
		:	[z] "r" (&zeros)
	);
	sync();
}

} // namespace libnux::vx
