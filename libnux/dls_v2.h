#pragma once

#include <stdint.h>

#include "omnibus.h"

using namespace libnux;

/* Size of synram */
static uint32_t const dls_num_rows = 32;
static uint32_t const dls_num_columns = 32;
static uint32_t const dls_num_synapses = 32 * 32;

/* Vector addressing of synapses */
static uint32_t const dls_num_synapse_vectors = 32 * 32 / 16;

/* Addressing for vector in/out */
static uint32_t const dls_weight_base = 0x0000;
static uint32_t const dls_decoder_base = 0x4000;
static uint32_t const dls_causal_base = 0x8000;
static uint32_t const dls_vreset_causal_base = 0x9000;
static uint32_t const dls_acausal_base = 0xc000;
static uint32_t const dls_vreset_acausal_base = 0xd000;

/* Byte to be written to synapse for correlation reset */
static uint8_t const dls_correlation_reset = 0x3;

/* Bitmask for weight decoding */
static uint32_t const dls_weight_mask = 0x3f;
static uint32_t const dls_decoder_mask = 0x3f;
static uint32_t const dls_buffer_enable_mask = 0x200000;
static uint32_t const dls_test_mask = 0x100000;


/*
 * Omnibus addresses
 * The following omnibus addresses are in the "FPGA omnibus tree format". To be used
 * on the PPU, the need to be modified using get_omnibus_pointer (see omnibus.h for details).
 */

/* Addressing of rate counters */
static constexpr omnibus_address_t dls_rates_base = 0x1e000000ul;

/* Address of synapse driver configuration */
static constexpr omnibus_address_t dls_syndrv_base = 0x1c000000ul;

/* Address for spike injection */
static constexpr omnibus_address_t dls_spike_base = 0x1c000040ul;

static constexpr omnibus_address_t dls_synapse_array_base = 0x0001f000ul;
