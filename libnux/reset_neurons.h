#pragma once
#include <s2pp.h>
#include "libnux/dls.h"

#ifdef LIBNUX_DLS_VERSION_VX
namespace libnux {

/**
 * Reset neurons (of hemisphere) where given mask is larger than zero.
 * @param mask_even Mask for even neuron positions
 * @param mask_odd Mask for odd neuron positions
 */
inline void reset_neurons(__vector uint8_t mask_even, __vector uint8_t mask_odd)
{
	// clang-format off
	asm volatile (
		"fxvcmpb %[mask_even]\n"
		"fxvoutx %[mask_even], %[base], %[index_even], %[gt]\n"
		"fxvcmpb %[mask_odd]\n"
		"fxvoutx %[mask_odd], %[base], %[index_odd], %[gt]\n"
		"sync\n"
		:
		: [mask_even] "qv" (mask_even),
		  [mask_odd] "qv" (mask_odd),
		  [base] "r" (dls_neuron_reset_base),
		  [index_even] "r" (0),
		  [index_odd] "r" (1),
		  [gt] "I" (__C_GT)
		:
	);
	// clang-format on
}

/**
 * Reset all neurons (of hemisphere).
 */
inline void reset_neurons()
{
	// clang-format off
	asm volatile (
		"fxvoutx %[zero], %[base], %[index_even]\n"
		"fxvoutx %[zero], %[base], %[index_odd]\n"
		"sync\n"
		:
		: [zero] "qv" (vec_splat_u8(0)),
		  [base] "r" (dls_neuron_reset_base),
		  [index_even] "r" (0),
		  [index_odd] "r" (1)
		:
	);
	// clang-format on
}

} // namespace libnux
#endif
