#include "libnux/omnibus.h"

using namespace libnux;

omnibus_word_t get_neuron_counter(uint8_t neuron)
{
	return omnibus_read(dls_rates_base + neuron);
}

void reset_neuron_counter(uint8_t neuron)
{
	omnibus_write(dls_rates_base + neuron, 0);
}

void reset_all_neuron_counters()
{
	uint8_t neuron;
	for (neuron = 0; neuron < dls_num_columns; neuron++) {
		reset_neuron_counter(neuron);
	}
}

void enable_neuron_counters(uint32_t enable_mask)
{
	omnibus_write(dls_rates_base + dls_num_columns, enable_mask);
}

omnibus_word_t get_enabled_neuron_counters()
{
	return omnibus_read(dls_rates_base + dls_num_columns);
}

void configure_neuron_counter(neuron_counter_config config)
{
	uint32_t scalar;
	scalar = config.fire_interrupt | (config.clear_on_read << 1);
	omnibus_write(dls_rates_base + dls_num_columns + 1, scalar);
}

neuron_counter_config get_neuron_counter_configuration()
{
	neuron_counter_config ret;
	auto scalar = omnibus_read(dls_rates_base + dls_num_columns + 1);
	ret.fire_interrupt = scalar & (1 << 0);
	ret.clear_on_read = (scalar & (1 << 1)) >> 1;
	return ret;
}

void clear_neuron_counters_on_read(bool value)
{
	neuron_counter_config tmp_config;
	tmp_config = get_neuron_counter_configuration();
	tmp_config.clear_on_read = value;
	configure_neuron_counter(tmp_config);
}

void fire_interrupt(bool value)
{
	neuron_counter_config tmp_config;
	tmp_config = get_neuron_counter_configuration();
	tmp_config.fire_interrupt = value;
	configure_neuron_counter(tmp_config);
}
