#include "libnux/vx/random.h"

namespace libnux::vx {

uint32_t xorshift32(uint32_t* seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return *seed;
}

/*
	Calculate cutoff:
	cutoff = target_freq * dt * 0xffffffff
 */
uint32_t draw_poisson(uint32_t* seed, uint32_t cutoff, uint32_t dt)
{
	uint32_t t = 0;
	xorshift32(seed);
	while (*seed > cutoff) {
		xorshift32(seed);
		t += dt;
	}
	return t;
}

uint32_t random_lcg(uint32_t *seed) {
  // constants from Numerical Recipes via Wikipedia
  uint32_t rv = 1664525 * (*seed) + 1013904223;
  *seed = rv;
  return rv;
}

} // namespace libnux::vx
