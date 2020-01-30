uint32_t get_neuron_counter(uint8_t neuron)
{
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + neuron);
	return *ptr;
}

void reset_neuron_counter(uint8_t neuron)
{
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + neuron);
	*ptr = 0;
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
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + dls_num_columns);
	*ptr = enable_mask;
}

uint32_t get_enabled_neuron_counters()
{
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + dls_num_columns);
	return *ptr;
}

void configure_neuron_counter(neuron_counter_config config)
{
	uint32_t scalar;
	scalar = config.fire_interrupt | (config.clear_on_read << 1);
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + dls_num_columns + 1);
	*ptr = scalar;
}

neuron_counter_config get_neuron_counter_configuration()
{
	neuron_counter_config ret;
	uint32_t scalar;
	volatile uint32_t* ptr = (uint32_t*) (dls_rates_base + dls_num_columns + 1);
	scalar = *ptr;
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
