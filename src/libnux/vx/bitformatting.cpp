#include <cstddef>
#include "libnux/vx/bitformatting.h"

namespace libnux::vx {

uint8_t reverse_byte(uint8_t b)
{
	// First switch the first 4 bits with the last 4 bits
	b = (uint8_t) ((b & 0xF0) >> 4 | (b & 0x0F) << 4);

	// Then switch packets of 2 bits
	b = (uint8_t) ((b & 0xCC) >> 2 | (b & 0x33) << 2);

	// At the end switch single bits to get the final result
	b = (uint8_t) ((b & 0xAA) >> 1 | (b & 0x55) << 1);

	return b;
}

void vector_reverse_bytes(__vector uint8_t* data)
{
	// Switch every entry in the vector individually
	for (size_t i = 0; i < sizeof(__vector uint8_t); i++) {
		(*data)[i] = reverse_byte((*data)[i]);
	}
}

} // namespace libnux::vx
