#pragma once

#include <s2pp.h>
#include <stdint.h>
#include "dls.h"

// FIXME: write test 
void reset_all_correlations();
//   Saves causal & acausal correlation of both halves of a synapse row
//   in arrays at `first_half`, `second half`, respectively.
static inline void get_correlation(
    __vector uint8_t* first_causal_half,
    __vector uint8_t* second_causal_half,
    __vector uint8_t* first_acausal_half,
    __vector uint8_t* second_acausal_half,
    uint8_t row)
{
	asm volatile(
		// clang-format off
		"fxvinx %[first_ca], %[ca_base], %[first_index]\n"
		"fxvinx %[second_ca], %[cab_base], %[second_index]\n"
		"fxvinx %[first_ac], %[ac_base], %[first_index]\n"
		"fxvinx %[second_ac], %[acb_base], %[second_index]\n"
#ifndef LIBNUX_DLS_VERSION_VX
		: [first_ca] "=kv" (*first_causal_half),
		  [second_ca] "=kv" (*second_causal_half),
		  [first_ac] "=kv" (*first_acausal_half),
		  [second_ac] "=kv" (*second_acausal_half)
#else
		: [first_ca] "=qv" (*first_causal_half),
		  [second_ca] "=qv" (*second_causal_half),
		  [first_ac] "=qv" (*first_acausal_half),
		  [second_ac] "=qv" (*second_acausal_half)
#endif
		: [ca_base] "r" (dls_causal_base),
		  [ac_base] "r" (dls_acausal_base),
		  [cab_base] "r" (dls_causal_base|dls_buffer_enable_mask),
		  [acb_base] "r" (dls_acausal_base|dls_buffer_enable_mask),
		  [first_index] "r" (row*2),
		  [second_index] "r" (row*2+1)
		: /* no clobber */
		// clang-format on
	);
}


static inline void get_causal_correlation(
    __vector uint8_t* first_half, __vector uint8_t* second_half, uint8_t row)
{
	asm volatile(
		// clang-format off
		"fxvinx %[first_ca], %[ca_base], %[first_index]\n"
		"fxvinx %[second_ca], %[cab_base], %[second_index]\n"
#ifndef LIBNUX_DLS_VERSION_VX
		: [first_ca] "=kv" (*first_half),
		  [second_ca] "=kv" (*second_half)
#else
		: [first_ca] "=qv" (*first_half),
		  [second_ca] "=qv" (*second_half)
#endif
		: [ca_base] "r" (dls_causal_base),
		  [cab_base] "r" (dls_causal_base|dls_buffer_enable_mask),
		  [first_index] "r" (row*2),
		  [second_index] "r" (row*2+1)
		: /* no clobber */
		// clang-format on
	);
}

static inline void reset_correlation(uint8_t row)
{
	__vector uint8_t reset_vec;
	reset_vec = vec_splat_u8(dls_correlation_reset);
	asm volatile(
		// clang-format off
		"fxvoutx %[select], %[ca_base], %[index_first]\n"
		"fxvoutx %[select], %[ca_base], %[index_second]"
		: /* no output */
#ifndef LIBNUX_DLS_VERSION_VX
		: [select] "kv" (reset_vec),
#else
		: [select] "qv" (reset_vec),
#endif
		  [ca_base] "r" (dls_causal_base),
		  [index_first] "r" (row*2),
		  [index_second] "r" (row*2+1)
		: /* no clobber */
		// clang-format on
	);
}
