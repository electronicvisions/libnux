#pragma once

#include <s2pp.h>
#include <stdint.h>
#include "dls.h"

// Get weights of synapse row `row` and save in vectors `first_half`, `second_half`.
inline void get_weights(__vector uint8_t* first_half, __vector uint8_t* second_half, uint8_t row)
{
	asm volatile(
		// clang-format off
		"fxvinx %[first], %[base], %[first_index]\n"
		"fxvinx %[second], %[base], %[second_index]"
#ifndef LIBNUX_DLS_VERSION_VX
		: [first] "=kv" (*first_half),
		  [second] "=kv" (*second_half)
#else
		: [first] "=qv" (*first_half),
		  [second] "=qv" (*second_half)
#endif
		: [base] "b" (dls_weight_base),
		  [first_index] "r" (row*2),
		  [second_index] "r" (row*2+1)
		: /* no clobber */
		// clang-format on
	);
}

inline void set_weights(__vector uint8_t* first_half, __vector uint8_t* second_half, uint8_t row)
{
	asm volatile(
		// clang-format off
		"fxvoutx %[first], %[base], %[first_index]\n"
		"fxvoutx %[second], %[base], %[second_index]"
		: /* no outputs */
#ifndef LIBNUX_DLS_VERSION_VX
		: [first] "kv" (*first_half),
		  [second] "kv" (*second_half),
#else
		: [first] "qv" (*first_half),
		  [second] "qv" (*second_half),
#endif
		  [base] "b" (dls_weight_base),
		  [first_index] "r" (row*2),
		  [second_index] "r" (row*2+1)
		: /* no clobber */
		// clang-format on
	);
}
