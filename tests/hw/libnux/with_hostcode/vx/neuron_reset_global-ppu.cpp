#include <cstdint>
#include "libnux/vx/dls.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

static constexpr uint8_t neuron_reset_payload = 0x2;


int start(void)
{
	vector_row_t dummy_payload;

	// Test full reset
	dummy_payload = neuron_reset_payload;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test no reset at all
	dummy_payload = 0;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test specific odd column reset
	dummy_payload = 0;
	dummy_payload.odd[10] = neuron_reset_payload;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	// Test specific even column reset
	dummy_payload = 0;
	dummy_payload.even[20] = neuron_reset_payload;
	set_row_via_vector(dummy_payload, 256, dls_causal_base);

	return 0;
}
