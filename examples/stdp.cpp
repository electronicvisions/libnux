#include <s2pp.h>
#include <cstdint>
#include "libnux/vx/correlation.h"
#include "libnux/vx/dls.h"
#include "libnux/vx/mailbox.h"

using namespace libnux::vx;

// Mailbox index at which the program "listens" for the commands, which are
// "wait", "update" and "stop"
static uint32_t const signal_addr_offset = 0xff0;
// Mailbox index from which the stdp strength scaling factor is read
static uint32_t const factor_addr_offset = 0xff4;

// The different signals
enum
{
	signal_wait = 0,
	signal_update = 1,
	signal_stop = 2,
};

// Measure correlation offsets
void measure_offsets(__vector uint8_t ca_offsets[], __vector uint8_t ac_offsets[])
{
	for (uint32_t index = 0; index < dls_num_synapse_vectors; index++) {
		// clang-format off
		asm volatile (
			"fxvinx 1, %[ca_base], %[index]\n"
			"fxvinx 2, %[ac_base], %[index]\n"
			"fxvshb %[ca_offset], 1, -1\n"
			"fxvshb %[ac_offset], 2, -1\n"
			: [ca_offset] "=qv" (ca_offsets[index]),
			  [ac_offset] "=qv" (ac_offsets[index])
			: [index] "r" (index),
			  [ca_base] "r" (dls_causal_base),
			  [ac_base] "r" (dls_acausal_base)
			: "qv1", "qv2");
		// clang-format on
	}
	asm volatile("sync");
	mailbox_write_string("Offsets measured\n");
}

// Update the weights:
// * Read the correlation measurements
// * Subtract the respective offsets
// * Scale the stdp update with the given factor
// * Apply the stdp update to the weights
void update_weights(
    uint8_t const factor, __vector uint8_t const ca_offsets[], __vector uint8_t const ac_offsets[])
{
	__vector uint8_t const select = vec_splat_u8(dls_correlation_reset);
	__vector uint8_t factors = vec_splat_u8(factor);
	__vector uint8_t zeros = vec_splat_u8(0);

	for (uint32_t index = 0; index < dls_num_synapse_vectors; index++) {
		// Registers are:
		// 1: causal measurement
		// 2: acausal measurement
		// 3: updates
		// 4: weights
		// clang-format off
		asm volatile (
			// Load causal and acausal measurement
			"fxvinx 1, %[ca_base], %[index]\n"
			"fxvinx 2, %[ac_base], %[index]\n"
			"fxvshb 1, 1, -1\n"
			"fxvshb 2, 2, -1\n"
			// Reset correlation measurement
			"fxvoutx %[select], %[ca_base], %[index]\n"
			// Subtract offsets
			"fxvsubbfs 1, 1, %[ca_offset]\n"
			"fxvsubbfs 2, 2, %[ac_offset]\n"
			// Calculate difference arg1 - arg2 and scale by factor
			"fxvsubbfs 3, 1, 2\n"
			"fxvmulbfs 3, 3, %[factors]\n"
			// Load the shifted weights
			"fxvinx 4, %[w_base], %[index]\n"
			"fxvshb 4, 4, 1\n"
			// Add to the weights
			"fxvaddbfs 4, 4, 3\n"
			// Set to zero if the result is smaller than 0
			"fxvcmpb 4\n"
			"fxvsel 4, 4, %[zeros], 2\n"
			// Save shifted weights
			"fxvshb 4, 4, -1\n"
			"fxvoutx 4, %[w_base], %[index]"
			: /* no output */
			: [index] "r" (index),
			  [ca_base] "r" (dls_causal_base),
			  [ac_base] "r" (dls_acausal_base),
			  [ca_offset] "qv" (ca_offsets[index]),
			  [ac_offset] "qv" (ac_offsets[index]),
			  [select] "qv" (select),
			  [w_base] "r" (dls_weight_base),
			  [zeros] "qv" (zeros),
			  [factors] "qv" (factors)
			: "qv1", "qv2", "qv3", "qv4");
		// clang-format on
	}
	asm volatile("sync");
	mailbox_write_string("Update done\n");
}

void print_weights(void)
{
	mailbox_write_string("Weights are\n");
	for (uint32_t index = 0; index < dls_num_synapse_vectors; index++) {
		uint8_t __vector weights;
		// clang-format off
		asm volatile (
			"fxvinx 1, %[dls_weight_base], %[index]\n"
			"fxvstax 1, %[weights_addr], 0\n"
			"sync\n"
			: /* no output */
			: [dls_weight_base] "r" (dls_weight_base),
			  [index] "r" (index),
			  [weights_addr] "r" (&weights)
			: "kv1");
		// clang-format on
		for (uint32_t j = 0; j < 16; j++) {
			mailbox_write_int(weights[j]);
			mailbox_write_string(" ");
		}
		if ((index % 2) == 1) {
			mailbox_write_string("\n");
		}
	}
}

int start(void)
{
	// Initialize
	reset_all_correlations();
	mailbox_write_string("Synapses reset done\n");

	// Measure offsets
	__vector uint8_t ca_offsets[dls_num_synapse_vectors];
	__vector uint8_t ac_offsets[dls_num_synapse_vectors];
	measure_offsets(ca_offsets, ac_offsets);

	// Execute
	uint8_t signal = signal_wait;
	do {
		signal = mailbox_read_u8(signal_addr_offset);
		if (signal == signal_update) {
			mailbox_write_u8(signal_addr_offset, signal_wait);
			uint8_t const factor = mailbox_read_u8(factor_addr_offset);
			update_weights(factor, ca_offsets, ac_offsets);
		}
	} while (signal != signal_stop);

	// Print the whole weight matrix at the end of the emulation
	print_weights();
	mailbox_write_string("Program exited gracefully\n");
	return 0;
}
