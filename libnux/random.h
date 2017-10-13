#pragma once

static uint32_t random_lcg(uint32_t *seed) {
	// constants from Numerical Recipes via Wikipedia
	uint32_t rv = 1664525 * (*seed) + 1013904223;
	*seed = rv;
	return rv;
}

