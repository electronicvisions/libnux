#pragma once
#include <s2pp.h>
#include "libnux/MaskWrapper.hpp"
#include "libnux/dls.h"

namespace PlasticityRules {

namespace VectorRules {
// PlasticityRule with vector_rule
//  - sets weights to constant value
//  - with checking a mask vector whether to update specific synapses.
class Constant
{
private:
	// specific rule parameter
	__vector uint8_t values;

public:
	Constant(uint8_t);
	void vector_rule(vector_synram_address const, __vector uint8_t const&);
};

Constant::Constant(uint8_t value)
{
	this->values = vec_splat_u8(value);
}

void Constant::vector_rule(vector_synram_address const addr, __vector uint8_t const& vec)
{
	// temporary storage for weights
	__vector uint8_t tmp;
	// vector update rule
	asm volatile(
	    // load current weights
	    "fxvinx %[tmp], %[dls_weight_base], %[index]\n"
	    // load mask in compare register
	    "fxvcmpb %[vec]\n"
	    // set weights to value if mask[i] > 0
	    "fxvsel %[tmp], %[tmp], %[values], 1\n"
	    // save updated weights
	    "fxvoutx %[tmp], %[dls_weight_base], %[index]\n"
	    : [tmp] "=&kv"(tmp)
	    : [vec] "kv"(vec), [index] "r"(addr), [dls_weight_base] "b"(dls_weight_base),
	      [values] "kv"(this->values)
	    :);
}
}

// PlasticityRule with vector_rule
//  - sets weights to constant value
//  - with checking a mask vector whether to update specific synapses.
template <class Mask>
using Constant = MaskWrapper<VectorRules::Constant, Mask>;

namespace Tagged {

template <class Mask>
using Constant = MaskWrapper<VectorRules::Constant, Mask>;

} // namespace Tagged

}
