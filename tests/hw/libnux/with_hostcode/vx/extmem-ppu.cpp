#include <cstddef>
#include <cstdint>
#include "libnux/vx/dls.h"

using namespace libnux::vx;


int start(void)
{
	for (size_t i = 0; i < 256; ++i) {
		uint8_t* ptr = reinterpret_cast<uint8_t*>(extmem_data_base + (1 << 16) + i);
		*ptr = i;
	}

	return 0;
}
