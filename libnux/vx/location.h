#pragma once

#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"

namespace libnux::vx {

/**
 * Get location of execution by local write, global read operations.
 * @note This function temporarily modifies local synram and is not safe under concurrent execution
 * of the same function on the other PPU.
 * @param ppu PPUOnDLS to describe location
 * @return Boolean value whether operation was successful
 */
inline bool get_location(PPUOnDLS& ppu)
{
	constexpr size_t row = 0;
	auto working = get_row_via_vector(row, dls_weight_base);

	auto const before = working.even_columns[0];

	working.even_columns[0] = 0;
	set_row_via_vector(working, row, dls_weight_base);
	auto const top_read_0 = get_row_via_omnibus(row, synram_top_base_address).even_columns[0];
	auto const bottom_read_0 = get_row_via_omnibus(row, synram_bottom_base_address).even_columns[0];

	working.even_columns[0] = 32;
	set_row_via_vector(working, row, dls_weight_base);
	auto const top_read_1 = get_row_via_omnibus(row, synram_top_base_address).even_columns[0];
	auto const bottom_read_1 = get_row_via_omnibus(row, synram_bottom_base_address).even_columns[0];

	bool valid = false;
	if ((top_read_0 == 0) && (top_read_1 == 32)) {
		ppu = PPUOnDLS::top;
		valid = true;
	} else if ((bottom_read_0 == 0) && (bottom_read_1 == 32)) {
		ppu = PPUOnDLS::bottom;
		valid = true;
	}
	working.even_columns[0] = before;
	set_row_via_vector(working, row, dls_weight_base);
	return valid;
}

} // namespace libnux::vx
