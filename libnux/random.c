#include "random.h"

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
