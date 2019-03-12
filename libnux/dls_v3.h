#pragma once

#include <stdint.h>

// FIXME: Add rate counter address, buffer_enable/test masks
/* Size of synram */
static uint32_t const dls_num_rows = 32;
static uint32_t const dls_num_columns = 32;
static uint32_t const dls_num_synapses = 32 * 32;

/* Vector addressing of synapses */
static uint32_t const dls_num_synapse_vectors = 32 * 32 / 16;

/* Amount of synapse drivers */
static uint32_t const amount_synapse_drivers = 32 / 2;

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

/* FIXME: What is this for */
static uint32_t const dls_randgen_base = 0xe000;

/* Omnibus addresses
 * The omnibus addressing from the PPU is shifted by 2 bits compared to the
 * addressing from the FPGA, plus setting the most significant bit. For the
 * addresses refer to Schemmel & Hartel 2017 (Specification of the Hicann-DLS,
 * repository hicann-dls-private, doc folder), 6.6. It can be downloaded from
 * https://brainscales-r.kip.uni-heidelberg.de:11443/job/doc_hicann-dls-doc.
 * The MSB selects between addressing the SRAM or the bus.
 * Reading and writing to the omnibus should be done by dereferencing 32 bit
 * pointers. Therefore, all omnibus addresses should be placed here in the
 * format
 * */

/* Addressing of rate counters */
static uint32_t* const dls_rates_base = (uint32_t*) ((0x12000800ul << 2) | (1ul << 31));
static uint32_t* const dls_rates_reset =
    (uint32_t*) (((0x12000800ul | (1ul << 10)) << 2) | (1ul << 31));

/* Address of synapse driver configuration */
static uint32_t* const dls_syndrv_base = (uint32_t*) ((0x14000000ul << 2) | (1ul << 31));
static uint32_t* const dls_syndrv_config_base =
    (uint32_t*) (((0x14000000ul | (1ul << 11)) << 2) | (1ul << 31));
static uint32_t* const dls_padi_fire =
    (uint32_t*) (((0x14000000ul | (1ul << 8)) << 2) | (1ul << 31));

/* Address of capacitive memory configuration */
static uint32_t* const dls_capmem_config_base = (uint32_t*) ((0x10010000ul << 2) | (1ul << 31));
static uint32_t* const dls_capmem_cell_base = (uint32_t*) ((0x10000000ul << 2) | (1ul << 31));

/* Address for spike injection */
static uint32_t* const dls_spike_base = (uint32_t*) ((0x15000000ul << 2) | (1ul << 31));

/* Address of digital neuron configuration for triggering reset */
static uint32_t* const dls_nrn_digital_mod = (uint32_t*) ((0x12000000ul << 2) | (1ul << 31));

/* Address of digital neuron configuration */
static uint32_t* const dls_nrn_digital_set =
    (uint32_t*) (((0x12000000ul | (1ul << 12)) << 2) | (1ul << 31));
static uint32_t* const dls_nrn_digital_clock_scale =
     (uint32_t*) (((0x12001000ul | (1ul << 11)) << 2) | (1ul << 31));

/* Address of synapse array configuration */
static uint32_t* const dls_synapse_array_base = (uint32_t*) ((0x0001f000ul << 2) | (1ul << 31));
