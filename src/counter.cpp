#include "libnux/counter.h"
#include "libnux/dls.h"


#ifdef LIBNUX_DLS_VERSION_V2
#include "counter_v2.cpp"
#endif

#ifdef LIBNUX_DLS_VERSION_V3
#include "counter_v3.cpp"
#endif

#if defined(LIBNUX_DLS_VERSION_V2) || defined(LIBNUX_DLS_VERSION_V3)
void reset_all_neuron_counters()
{
	uint8_t neuron;
	for (neuron = 0; neuron < dls_num_columns; neuron++) {
		reset_neuron_counter(neuron);
	}
}
#endif
