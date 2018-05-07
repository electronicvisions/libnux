#pragma once
#include <s2pp.h>
#include "libnux/MaskWrapper.hpp"

namespace PlasticityRules {

namespace VectorRules {

// VectorRule with empty vector_rule
class Stub
{
public:
	Stub() {}
	void vector_rule(vector_synram_address const, __vector uint8_t const&) {}
};
}

// PlasticityRule with empty vector_rule
template <typename Mask>
using Stub = MaskWrapper<VectorRules::Stub, Mask>;

namespace Tagged {
template <typename Mask>
using Stub = MaskWrapper<VectorRules::Stub, Mask>
} // namespace Tagged

}
