#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/dls.h"
#include "libnux/vector.h"


using namespace libnux;


void fill_array(vector_type& array, uint8_t target)
{
	for (auto& value : array) {
		value = target;
	}
}

enum reset_payload
{
	causal = 0x1,
	acausal = 0x2
};


int start(void)
{
	vector_row_t dummy_payload;

	// Test full reset
	fill_array(dummy_payload.even_columns, reset_payload::causal | reset_payload::acausal);
	fill_array(dummy_payload.odd_columns, reset_payload::causal | reset_payload::acausal);
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 10, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test causal-only reset
	fill_array(dummy_payload.even_columns, reset_payload::causal);
	fill_array(dummy_payload.odd_columns, reset_payload::causal);
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 20, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test acausal-only reset
	fill_array(dummy_payload.even_columns, reset_payload::acausal);
	fill_array(dummy_payload.odd_columns, reset_payload::acausal);
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 30, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test no reset at all
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 40, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test specific odd column reset
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	dummy_payload.odd_columns[10] = reset_payload::causal;
	dummy_payload.odd_columns[10] = reset_payload::causal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 50, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test specific even column reset
	fill_array(dummy_payload.even_columns, 0);
	fill_array(dummy_payload.odd_columns, 0);
	dummy_payload.even_columns[20] = reset_payload::causal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 60, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	return 0;
}
