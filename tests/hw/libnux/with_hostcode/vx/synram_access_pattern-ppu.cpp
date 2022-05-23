#include <cstddef>
#include <cstdint>
#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

int start(void)
{
	vector_row_t tmp_row;
	tmp_row.fill(0);

	// Fill diagonal with ascending values between 1 and max_weight
	uint8_t diagonal_value = 1;
	for (size_t row_id = 0; row_id < dls_num_rows; ++row_id) {
		if (row_id % 2 == 0) {
			tmp_row.odd_columns = vec_splat_u8(0);
			tmp_row.even_columns[row_id / 2] = diagonal_value;
		} else {
			tmp_row.even_columns = vec_splat_u8(0);
			tmp_row.odd_columns[row_id / 2] = diagonal_value;
		}

		set_row_via_vector(tmp_row, row_id, dls_weight_base);

		diagonal_value++;
		if (diagonal_value > dls_weight_mask) {
			diagonal_value = 1;
		}
	}

	return 0;
}
