#include <cstdint>
#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

enum reset_payload
{
	causal = 0x1,
	acausal = 0x2
};


int start(void)
{
	vector_row_t dummy_payload;

	// Test full reset
	dummy_payload = (reset_payload::causal | reset_payload::acausal);
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 10, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test causal-only reset
	dummy_payload = reset_payload::causal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 20, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test acausal-only reset
	dummy_payload = reset_payload::acausal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 30, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test no reset at all
	dummy_payload = 0;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 40, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test specific odd column reset
	dummy_payload = 0;
	dummy_payload.odd[10] = reset_payload::causal;
	dummy_payload.odd[10] = reset_payload::causal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 50, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	// Test specific even column reset
	dummy_payload = 0;
	dummy_payload.even[20] = reset_payload::causal;
	set_row_via_vector(dummy_payload, 0, dls_causal_base);
	set_row_via_vector(dummy_payload, 60, dls_causal_base);
	set_row_via_vector(dummy_payload, 255, dls_causal_base);

	return 0;
}
