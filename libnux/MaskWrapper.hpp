#pragma once
#include <stdint.h>

namespace PlasticityRules {

// Mask Wrapper provides:
// - associated Mask, i.e. on which synapses the Plasticity Vector Rule runs
// - associated VectorRule, which modifies weights on a vector scale,
//   masked by correct vector from mask
// - run() calls apply_vector_rule with vector_rule from VectorRule
//   altering only synapses present in Mask
template <class VectorRule, typename Mask>
class MaskWrapper : public VectorRule
{
private:
	Mask& mask;

public:
	template <class... T>
	MaskWrapper(Mask& m, T... t);
	void run();
};

template <class VectorRule, typename Mask>
template <class... T>
MaskWrapper<VectorRule, Mask>::MaskWrapper(Mask& mask, T... t) : VectorRule(t...), mask(mask)
{}

template <class VectorRule, typename Mask>
void MaskWrapper<VectorRule, Mask>::run()
{
	this->mask.apply_vector_rule(static_cast<VectorRule*>(this), &VectorRule::vector_rule);
}

namespace Tagged {

// Mask Wrapper provides:
// - associated Mask, i.e. tag information of the synapses
// - associated VectorRule, which modifies weights on a vector scale,
//   masked by correct vector from mask
// - run() calls apply_vector_rule with vector_rule from VectorRule
//   altering only synapses present in Mask
// - tag to decide on which synapses in Mask the rule runs
template <class VectorRule, typename Mask>
class MaskWrapper : public VectorRule
{
private:
	Mask& mask;
	const uint8_t tag;

public:
	template <class... T>
	MaskWrapper(Mask& m, uint8_t tag, T... t);
	void run();
};

template <class VectorRule, typename Mask>
template <class... T>
MaskWrapper<VectorRule, Mask>::MaskWrapper(Mask& mask, uint8_t tag, T... t) :
    VectorRule(t...),
    mask(mask),
    tag(tag)
{}

template <class VectorRule, typename Mask>
void MaskWrapper<VectorRule, Mask>::run()
{
	this->mask.apply_vector_rule(static_cast<VectorRule*>(this), &VectorRule::vector_rule, tag);
}

} // namespace Tagged

} // namespace PlasticityRules
