#include <s2pp.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/random.h"

using namespace libnux::vx;

// Use 2 of 32 vectors on HX fitting in the vector registers
// For more than 32 there is not enough memory
// 2 is used in order to reduce the runtime in simulation
#define SYNAPSE_ROW_ITERATIONS 2

void set_synram_random(uint32_t const base_address, uint32_t const mask, uint32_t seed) {
	/* Initialize data in the synram */
	__vector uint8_t data[SYNAPSE_ROW_ITERATIONS];
	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++) {
		for (uint32_t component = 0; component < sizeof(__vector uint8_t); component++) {
			data[index][component] = xorshift32(&seed) & mask;
		}
	}

	/* Memory barrier to ensure all write operations finished: no instruction,
	 * no output and no input, but memory clobbered. */
	asm volatile ("" : : : "memory");

	/* Set the vector in the synram */
	__vector uint8_t* d_it = data;
	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++, d_it++) {
		asm volatile (
				"fxvoutx %[vec], %[base], %[index]"
				: /* no output */
				: [vec] "qv" (*d_it),
				  [base] "b" (base_address),
				  [index] "r" (index)
				: /* no clobbers */);
	}

	/* Wait for all vector instructions and r/w operations to finish */
	asm volatile ("sync");
}


void test_weight_read(uint32_t seed) {
	testcase_begin("test_weight_read");

	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++) {
		/* Explicitely load the weights, store to memory and synchronize */
		__vector uint8_t data;
		__vector uint8_t temp;
		asm volatile (
				"fxvinx %[temp], %[base], %[index]\n"
				"fxvstax %[temp], 0, %[addr]\n"
				"sync"
				: [temp] "=&qv" (temp)
				: [base] "b" (dls_weight_base),
				  [index] "r" (index),
				  [addr] "r" (&data)
				: /* no clobbers */);
		for (uint32_t j = 0; j < sizeof(__vector uint8_t); j++) {
			test_equal(data[j], xorshift32(&seed) & dls_weight_mask);
		}
	}

	testcase_end();
}


void test_decoder_read(uint32_t seed) {
	testcase_begin("test_decoder_read");

	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++) {
		/* Explicitely load the weights, store to memory and synchronize */
		__vector uint8_t data;
		__vector uint8_t temp;
		asm volatile (
				"fxvinx %[temp], %[base], %[index]\n"
				"fxvstax %[temp], 0, %[addr]\n"
				"sync"
				: [temp] "=&qv" (temp)
				: [base] "b" (dls_decoder_base),
				  [index] "r" (index),
				  [addr] "r" (&data)
				: /* no clobbers */);
		for (uint32_t j = 0; j < sizeof(__vector uint8_t); j++) {
			test_equal(data[j], xorshift32(&seed) & dls_decoder_mask);
		}
	}

	testcase_end();
}


void start(void) {
	test_init();
	set_synram_random(dls_weight_base, dls_weight_mask, 42);
	set_synram_random(dls_decoder_base, dls_decoder_mask, 1);
	test_weight_read(42);
	test_decoder_read(1);
	test_summary();
	test_shutdown();
}
