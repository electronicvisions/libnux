#include "libnux/Mask.hpp"
#include "libnux/PlasticityRule_STDP.hpp"

// initialize PPU-program
int start()
{
	// initialize PlasticityRule
	uint8_t synapse_factor = 32;
	uint8_t decay_factor = 48;
	uint8_t update_scale = 32;
#ifndef LIBNUX_DLS_VERSION_VX
	__vector uint8_t random_seed = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
#else
	__vector uint8_t random_seed;
	for (size_t i = 0; i < 128; ++i) {
		random_seed[i] = i;
	}
#endif
	// set arbitrary Mask
	// 17 partial vectors, 3 full vectors
	Mask<17, 3> mask;

	// set mask values
	// full vectors with addresses 17-19
	for (uint32_t full = 0; full < 3; ++full) {
		mask.full_vector_synram_address[full] = full + 17;
	}
	// partial vectors
	// mask.vectors[vec] =
	//	(vec % 3 == 0 ? {1,0,0,...} : {0,1,1,...} )
	// addresses from 0 to 16
	for (uint32_t vec = 0; vec < 17; ++vec) {
		mask.partial_vector_synram_address[vec] = vec;
		if (vec % 3 == 0) {
			mask.vectors[vec] = vec_splat_u8(0);
			mask.vectors[vec][0] = 1;
		} else {
			mask.vectors[vec] = vec_splat_u8(1);
			mask.vectors[vec][0] = 0;
		}
	}

	// create plasticity rule with given parameters and mask
	auto rule = PlasticityRules::STDP<Mask<17, 3> >(
	    mask, synapse_factor, decay_factor, update_scale, random_seed);

	// run plasticity rule once
	rule.run();

	return 0;
}
