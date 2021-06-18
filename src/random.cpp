#include "libnux/dls.h"
#include "libnux/random.h"
#include "libnux/vector_helper.h"

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

#ifndef LIBNUX_DLS_VERSION_VX
// ~3 times faster than calling xorshift32 four times.
void xorshift_vector(const __vector uint8_t* seed)
{
	// Check if vector is 16B long. Otherwise xorshift_vector won't work
	// because it assumes exactly 4 uint32_t to fit in a vector.
	static_assert(
	    sizeof(__vector uint8_t) == 16, "vector size is not 16B, xorshift_vector will not work.");

	// xorshift128
	uint32_t* x = (uint32_t*) seed;
	uint32_t t = *x ^ (*x << 11);
	*x = *(x + 1);
	*(x + 1) = *(x + 2);
	*(x + 2) = *(x + 3);
	*(x + 3) ^= (*(x + 3) >> 19) ^ t ^ (t >> 8);
}
#endif

uint32_t random_lcg(uint32_t *seed) {
  // constants from Numerical Recipes via Wikipedia
  uint32_t rv = 1664525 * (*seed) + 1013904223;
  *seed = rv;
  return rv;
}

#ifdef LIBNUX_DLS_VERSION_VX

/*
	Initalize random register to "start" random number generation in
	a specified row. Seed has to be unequal to zero.
	Possible are all rows between 0 and 2047.
	Initialization can also be done via Omnibus.
	Every uint8_t entry has to be unequal to zero, otherwise the according
	entry will never differ from zero.
*/
void init_random_generator(size_t row, uint8_t seed)
{
	__vector uint8_t vec = vec_splat_u8(seed);

	asm volatile(
		"fxvoutx %[vec], %[addr], %[row]\n"
		"sync\n"
		:
		: [vec] "qv" (vec),
		  [addr] "b" (dls_randgen_base),
		  [row] "r" (row)
	);
}
void init_random_generator(size_t row, __vector uint8_t seed_vec)
{
	asm volatile(
		"fxvoutx %[vec], %[addr], %[row]\n"
		"sync\n"
		:
		: [vec] "qv" (seed_vec),
		  [addr] "b" (dls_randgen_base),
		  [row] "r" (row)
	);
}

/*
	If the random generator is seeded with the same integer on every entry
	one needs some calls to spread them from each other.
*/
void make_rand_calls(size_t row, size_t num)
{
	__vector uint8_t vec;

	for(size_t i = 0; i < num; i++) {
		asm volatile(
			"fxvinx %[vec], %[addr], %[row]\n"
			: [vec] "=qv" (vec)
			: [addr] "b" (dls_randgen_base),
			  [row] "r" (row)
		);
	}
}

/*
	Get random numbers in vector from a certain row.
*/
#define GET_RAND(type, name)\
__vector type get_rand_##name(size_t row)\
{\
	__vector type vec;\
\
	asm volatile(\
		"fxvinx %[vec], %[addr], %[row]\n"\
		"sync\n"\
		: [vec] "=qv" (vec)\
		: [addr] "b" (dls_randgen_base),\
		  [row] "r" (row)\
		:\
	);\
\
	return vec;\
}

GET_RAND(int8_t, s8)
GET_RAND(uint8_t, u8)
GET_RAND(uint16_t, u16)
#undef GET_RAND

/*
	Returns a random amount of bits inside the vector.
	For example with bit equal to 4 the values in the return vector lie
	within 0 and 15 (0 -- (2**4 - 1)).
*/
#define GET_RAND_BITWISE(type, name)\
__vector type get_rand_bitwise_##name(size_t row, size_t bit)\
{\
	__vector type rand = get_rand_##name(row);\
\
	shift_vector_right_##name(rand, sizeof(type)*8 - bit);\
\
	return rand;\
}

GET_RAND_BITWISE(uint8_t, u8)
GET_RAND_BITWISE(uint16_t, u16)
#undef GET_RAND_BITWISE

#endif
