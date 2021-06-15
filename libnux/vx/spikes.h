#pragma once

#include <stdint.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/omnibus.h"
#include "libnux/vx/time.h"

namespace libnux::vx {

typedef struct
{
	uint32_t row_mask;
	uint8_t addr;
} spike_t;

static inline void send_spike(spike_t* sp)
{
	omnibus_write(dls_spike_base + (sp->addr), sp->row_mask);
}

void send_uniform_spiketrain(spike_t* single_spike, uint32_t number, uint32_t isi_usec);

} // namespace libnux::vx
