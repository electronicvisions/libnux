#include <s2pp.h>
#include "libnux/dls.h"
#include "libnux/unittest.h"
#include "libnux/random.h"


#ifndef LIBNUX_DLS_VERSION_VX
#define SYNAPSE_ROW_ITERATIONS dls_num_synapse_vectors
#else
// Use 32 vectors on HX fitting in the vector registers
// Otherwise there is not enough memory
#define SYNAPSE_ROW_ITERATIONS 32
#endif

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
#ifndef LIBNUX_DLS_VERSION_VX
				: [vec] "kv" (*d_it),
#else
				: [vec] "qv" (*d_it),
#endif
				  [base] "b" (base_address),
				  [index] "r" (index)
				: /* no clobbers */);
	}

	/* Wait for all vector instructions and r/w operations to finish */
	asm volatile ("sync");
}


void test_weight_read(uint32_t seed) {
	libnux_testcase_begin("test_weight_read");

	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++) {
		/* Explicitely load the weights, store to memory and synchronize */
		__vector uint8_t data;
		__vector uint8_t temp;
		asm volatile (
				"fxvinx %[temp], %[base], %[index]\n"
				"fxvstax %[temp], 0, %[addr]\n"
				"sync"
#ifndef LIBNUX_DLS_VERSION_VX
				: [temp] "=&kv" (temp)
#else
				: [temp] "=&qv" (temp)
#endif
				: [base] "b" (dls_weight_base),
				  [index] "r" (index),
				  [addr] "r" (&data)
				: /* no clobbers */);
		for (uint32_t j = 0; j < sizeof(__vector uint8_t); j++) {
			libnux_test_equal(data[j], xorshift32(&seed) & dls_weight_mask);
		}
	}

	libnux_testcase_end();
}


void test_decoder_read(uint32_t seed) {
	libnux_testcase_begin("test_decoder_read");

	for (uint32_t index = 0; index < SYNAPSE_ROW_ITERATIONS; index++) {
		/* Explicitely load the weights, store to memory and synchronize */
		__vector uint8_t data;
		__vector uint8_t temp;
		asm volatile (
				"fxvinx %[temp], %[base], %[index]\n"
				"fxvstax %[temp], 0, %[addr]\n"
				"sync"
#ifndef LIBNUX_DLS_VERSION_VX
				: [temp] "=&kv" (temp)
#else
				: [temp] "=&qv" (temp)
#endif
				: [base] "b" (dls_decoder_base),
				  [index] "r" (index),
				  [addr] "r" (&data)
				: /* no clobbers */);
		for (uint32_t j = 0; j < sizeof(__vector uint8_t); j++) {
			libnux_test_equal(data[j], xorshift32(&seed) & dls_decoder_mask);
		}
	}

	libnux_testcase_end();
}


void start(void) {
	libnux_test_init();
	set_synram_random(dls_weight_base, dls_weight_mask, 42);
	set_synram_random(dls_decoder_base, dls_decoder_mask, 1);
	test_weight_read(42);
	test_decoder_read(1);
	libnux_test_summary();
	libnux_test_shutdown();
}
