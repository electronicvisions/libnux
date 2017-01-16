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
