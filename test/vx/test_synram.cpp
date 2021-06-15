#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/location.h"
#include "libnux/vx/omnibus.h"
#include "libnux/vx/sync.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

// weight value which is the same value than written, when read from Omnibus
constexpr uint8_t invariant_weight = 32;

/**
 * Write single byte via vector unit, all other bytes are disabled via byte enables.
 */
void write_byte_vector_unit(uint32_t row, size_t index, uint8_t value)
{
	vector_row_t values;
	// only one byte written -> other byte don't need initialization
	values[index] = value;

	vector_row_t mask;
	mask.fill(0);
	mask[index] = 1;
	sync();
	set_row_via_vector_masked(values, mask, row, dls_weight_base);
}

void start(void)
{
	test_init();

	PPUOnDLS ppu;
	bool const success = get_location(ppu);
	if (!success) {
		exit(1);
	}
	uint32_t synram_base =
	    (ppu == PPUOnDLS::top) ? synram_top_base_address : synram_bottom_base_address;

	// only use first row for now
	constexpr uint32_t row = 0;
	// zero columns
	vector_row_t zero;
	zero.fill(0);
	set_row_via_vector(zero, row, dls_weight_base);

	testcase_begin("synram fill linear");
	// write one weight at ascending index via byte-enables with a stride of 13,
	// which should essentially lead to filling up of the weight matrix like
	// t
	// | 32 0 ... 0  0  0 ...
	// v 32 0 ... 0 32  0 ...
	// ...
	vector_row_t expectation;
	expectation.fill(0);
	for (size_t column = 0; column < dls_num_columns; column += 13) {
		write_byte_vector_unit(row, column, invariant_weight);

		expectation[column] = invariant_weight;
		// read-out via Omnibus
		vector_row_t values = get_row_via_omnibus(row, synram_base);
		// Compare to expectation from Omnibus access
		test_equal(values, expectation);
	}

	// Test cornercase of last column
	constexpr size_t last_column = dls_num_columns - 1;
	write_byte_vector_unit(row, last_column, invariant_weight);
	expectation[last_column] = invariant_weight;
	vector_row_t values = get_row_via_omnibus(row, synram_base);
	test_equal(values, expectation);

	testcase_end();

	test_summary();
	test_shutdown();
}
