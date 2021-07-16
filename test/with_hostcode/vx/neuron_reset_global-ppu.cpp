#include <array>
#include <cstddef>
#include <cstdint>
#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

void fill_array(vector_type& array, uint8_t target)
{
	for (auto& value : array) {
		value = target;
	}
}

static constexpr uint8_t neuron_reset_payload = 0x2;


int start(void)
{
	vector_row_t dummy_payload;

	// Test full reset
	fill_array(dummy_payload.even_columns, neuron_reset_payload);
	fill_array(dummy_payload.odd_columns, neuron_reset_payload);
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test no reset at all
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test specific odd column reset
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	dummy_payload.odd_columns[10] = neuron_reset_payload;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test specific even column reset
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	dummy_payload.even_columns[20] = neuron_reset_payload;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	return 0;
}
