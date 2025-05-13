#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"
#include <cstddef>
#include <cstdint>

using namespace libnux::vx;


int start(void)
{
	vector_type values;
	for (size_t entry = 0; entry < dls_vector_size; ++entry) {
		values[entry] = dls_vector_size - (entry & 0xfc) - (4 - (entry & 0x3));
	}

	for (auto base : {extmem_data_base, extmem_dram_data_base}) {
		for (size_t i = 0; i < 256; ++i) {
			uint8_t* ptr = reinterpret_cast<uint8_t*>(base + (1 << 16) + i);
			*ptr = i;
		}
	}

	for (auto vector_base : {dls_extmem_base, dls_extmem_dram_base}) {
		set_vector(values, vector_base + (1 << 12) + (256 >> 4), 0);
	}

	return 0;
}
