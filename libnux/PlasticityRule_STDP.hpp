#pragma once
#include <s2pp.h>
#include "libnux/MaskWrapper.hpp"
#include "libnux/dls.h"
#include "libnux/random.h"

namespace PlasticityRules {

namespace VectorRules {

// PlasticityRule with vector_rule
// provides meaningful implementation of vector weights update
// with checking a mask vector whether to update for specific synapse
class STDP
{
private:
	uint8_t m_synapse_factor;
	uint8_t m_decay_factor;
	uint8_t m_update_scale;

	__vector uint8_t m_random;

public:
	STDP(
	    uint8_t synapse_factor,
	    uint8_t decay_factor,
	    uint8_t update_scales,
	    __vector uint8_t random_seed);

	void vector_rule(vector_synram_address, __vector uint8_t const& vec);
};

STDP::STDP(
    uint8_t synapse_factor, uint8_t decay_factor, uint8_t update_scale, __vector uint8_t random_seed)
{
	m_synapse_factor = synapse_factor;
	m_decay_factor = decay_factor;
	m_update_scale = update_scale;

	m_random = random_seed;
}

void STDP::vector_rule(vector_synram_address addr, __vector uint8_t const& mask)
{
#ifndef LIBNUX_DLS_VERSION_VX
	xorshift_vector(&m_random);
#else
	// Get random vecotr from rng 0. FIXME: Inititalize rng
	m_random = get_rand_u8(0);
#endif
	// Declare temporary variables
	__vector uint8_t temps;
	__vector uint8_t updates;
	__vector uint8_t weights;

	asm volatile(
	    // load weights
	    "fxvinx %[weights], %[dls_weight_base], %[index]\n"
	    // Get stdp update
	    "fxvinx %[temps], %[dls_causal_base], %[index]\n"
	    "fxvshb %[temps], %[temps], -1\n"
	    "fxvmulbfs %[updates], %[temps], %[synapse_factors]\n"
	    // Reset correlation measurement *masked*
	    "fxvinx %[temps], %[dls_causal_base], %[index]\n"
	    "fxvcmpb %[mask]\n"
	    "fxvsel %[temps], %[temps], %[select],1\n"
	    "fxvoutx %[temps], %[dls_causal_base], %[index]\n"
	    // Load the shifted weights
	    "fxvshb %[weights], %[weights], 1\n"
	    // Add decay update
	    "fxvmulbfs %[temps], %[weights], %[decay_factors]\n"
	    "fxvaddbfs %[updates], %[updates], %[temps]\n"
	    // Add random update
	    "fxvaddbfs %[updates], %[updates], %[random]\n"
	    // Divide and add the updates to the weights
	    "fxvmulbfs %[updates], %[updates], %[update_scales]\n"
	    "fxvaddbfs %[weights], %[weights], %[updates]\n"
	    // Set to zero if the result is smaller than 0
	    "fxvcmpb %[weights]\n"
	    "fxvsel %[weights], %[weights], %[zeros], 2\n"
	    // Shift weights
	    "fxvshb %[weights], %[weights], -1\n"
	    // Save weights *masked*
	    "fxvinx %[temps], %[dls_weight_base], %[index]\n"
	    "fxvcmpb %[mask]\n"
	    "fxvsel %[temps], %[temps], %[weights],1\n"
	    "fxvoutx %[temps], %[dls_weight_base], %[index]\n"
#ifndef LIBNUX_DLS_VERSION_VX
	    : [temps] "=&kv"(temps), [updates] "=&kv"(updates), [weights] "=&kv"(weights)
	    : [index] "r"(addr), [dls_causal_base] "b"(dls_causal_base),
	      [dls_weight_base] "b"(dls_weight_base),
	      [select] "kv"(vec_splat_u8(dls_correlation_reset)), [zeros] "kv"(vec_splat_u8(0)),
	      [decay_factors] "kv"(vec_splat_u8(m_decay_factor)),
	      [synapse_factors] "kv"(vec_splat_u8(m_synapse_factor)),
	      [update_scales] "kv"(vec_splat_u8(m_update_scale)), [random] "kv"(m_random),
	      [mask] "kv"(mask)
#else
	    : [temps] "=&qv"(temps), [updates] "=&qv"(updates), [weights] "=&qv"(weights)
	    : [index] "r"(addr), [dls_causal_base] "b"(dls_causal_base),
	      [dls_weight_base] "b"(dls_weight_base),
	      [select] "qv"(vec_splat_u8(dls_correlation_reset)), [zeros] "qv"(vec_splat_u8(0)),
	      [decay_factors] "qv"(vec_splat_u8(m_decay_factor)),
	      [synapse_factors] "qv"(vec_splat_u8(m_synapse_factor)),
	      [update_scales] "qv"(vec_splat_u8(m_update_scale)), [random] "qv"(m_random),
	      [mask] "qv"(mask)
#endif
	    :);
}

} // namespace VectorRules

template <class Mask>
using STDP = MaskWrapper<VectorRules::STDP, Mask>;

namespace Tagged {

template <class Mask>
using STDP = MaskWrapper<VectorRules::STDP, Mask>;

} // namespace Tagged

} // namespace PlasticityRules
