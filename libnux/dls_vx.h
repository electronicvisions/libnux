#pragma once

#include <stdint.h>

#include "omnibus.h"

using namespace libnux;

/* Size of synram */
constexpr static uint32_t dls_num_rows = 256;
constexpr static uint32_t dls_num_columns = 256;
constexpr static uint32_t dls_num_synapses = dls_num_rows * dls_num_columns;

/* Vector size */
constexpr static uint32_t dls_vector_size = 128;

/* Number of vectors per row */
constexpr static uint32_t dls_num_vectors_per_row = 2;

/* Vector addressing of synapses */
constexpr static uint32_t dls_num_synapse_vectors = dls_num_synapses / dls_vector_size;

/* Addressing for vector in/out */
static uint32_t const dls_weight_base         = 0x00000000;
static uint32_t const dls_decoder_base        = 0x00040000;
static uint32_t const dls_causal_base         = 0x00080000;
static uint32_t const dls_vreset_causal_base  = 0x00090000;
static uint32_t const dls_acausal_base        = 0x000c0000;
static uint32_t const dls_vreset_acausal_base = 0x000d0000;
static uint32_t const dls_config_even_base    = 0x00010000;
static uint32_t const dls_config_odd_base     = 0x00020000;
static uint32_t const dls_raw_base            = 0x000f0000;
static uint32_t const dls_randgen_base        = 0x000e0000;
static uint32_t const dls_extmem_base         = 0x80000000;

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

/* MADC and reference current generator configuration base address */
static constexpr omnibus_address_t madc_base_address = (1ul << 19 | 1ul << 18);

/* Address of synram */
static constexpr omnibus_address_t synram_top_base_address = 0x02800000 | (1 << 22);
static constexpr omnibus_address_t synram_bottom_base_address = 0x03800000 | (1 << 22);

/* CADC causal base address for top PPU */
static constexpr omnibus_address_t cadc_top_causal_base_address =
    synram_top_base_address | dls_causal_base;

/* CADC acausal base address for top PPU */
static constexpr omnibus_address_t cadc_top_acausal_base_address =
    synram_top_base_address | dls_acausal_base;
