#pragma once

#include <stdint.h>

/* Size of synram */
uint32_t const dls_num_rows = 32;
uint32_t const dls_num_columns = 32;
uint32_t const dls_num_synapses = 32 * 32;

/* Vector addressing of synapses */
uint32_t const dls_num_synapse_vectors = 32 * 32 / 16;

/* Addressing for vector in/out */
uint32_t const dls_weight_base = 0x0000;
uint32_t const dls_decoder_base = 0x4000;
uint32_t const dls_causal_base = 0x8000;
uint32_t const dls_vreset_causal_base = 0x9000;
uint32_t const dls_acausal_base = 0xc000;
uint32_t const dls_vreset_acausal_base = 0xd000;

/* Bitmask for weight decoding */
uint32_t const dls_weight_mask = 0x3f;
uint32_t const dls_decoder_mask = 0x3f;

/* Omnibus addresses
 * The omnibus addressing from the PPU is shifted by 2 bits compared to the
 * addressing from the FPGA, plus setting the most significant bit. For the
 * addresses refer to Hartel 2017, 6.2.1. The MSB selects between addressing
 * the SRAM or the bus.
 * Reading and writing to the omnibus should be done by dereferencing 32 bit
 * pointers. Therefore, all omnibus addresses should be placed here in the
 * format
 *
 * uint32_t* const dls_module_base = (uint32_t*)((address << 2) | (1 << 31));
 *
 * */

/* Addressing of rate counters */
uint32_t* const dls_rates_base = (uint32_t*)((0x1e000000 << 2) | (1 << 31));
/* Address of synapse driver configuration */
uint32_t* const dls_syndrv_base = (uint32_t*)((0x1c000000 << 2) | (1 << 31));
