#include "libnux/vx/mailbox.h"
#include "libnux/vx/parallel_rng.h"
#include "libnux/vx/random.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_frac_sat_16.h"
#include "libnux/vx/vector_half_row_mod_16.h"
#include "libnux/vx/vector_round_stochastic.h"
#include "libnux/vx/vector_row.h"
#include <array>

constexpr static size_t num_tests = 10;

TEST(VectorRoundStochastic, VectorHalfRowMod16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 16;
		size_t const val = xorshift32(&seed) % (1ul << 16);
		VectorHalfRowMod16 value(val);
		uint16_t const value_rounded_down = (static_cast<uint16_t>(val) >> num_bits) << num_bits;
		uint16_t const value_rounded_up =
		    (static_cast<uint16_t>(val >> num_bits) + static_cast<uint16_t>(1)) << num_bits;
		VectorHalfRowMod16 const rounded_value = vector_round_stochastic(value, num_bits);
		for (size_t j = 0; j < rounded_value.size; ++j) {
			EXPECT_TRUE(
			    rounded_value[j] == value_rounded_down || rounded_value[j] == value_rounded_up);
		}
	}
}

TEST(VectorRoundStochastic, VectorHalfRowFracSat16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 15;
		int const val = static_cast<int>(xorshift32(&seed) % (1ul << 16)) - (1 << 15);
		VectorHalfRowFracSat16 value(val);
		int16_t const value_rounded_down = (static_cast<int16_t>(val) >> num_bits) << num_bits;
		int const value_rounded_up_full =
		    ((val >> num_bits) << num_bits) + (static_cast<int16_t>(1) << num_bits);
		int16_t const value_rounded_up =
		    (((value_rounded_up_full < -(1 << 15))
		          ? -(1 << 15)
		          : ((value_rounded_up_full >= (1 << 15)) ? ((1 << 15) - 1)
		                                                  : value_rounded_up_full)) >>
		     num_bits)
		    << num_bits;
		VectorHalfRowFracSat16 const rounded_value = vector_round_stochastic(value, num_bits);
		for (size_t j = 0; j < rounded_value.size; ++j) {
			EXPECT_TRUE(
			    rounded_value[j] == value_rounded_down || rounded_value[j] == value_rounded_up);
		}
	}
}

TEST(VectorRoundStochastic, VectorRowMod16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 16;
		size_t const val = xorshift32(&seed) % (1ul << 16);
		VectorRowMod16 value(val);
		uint16_t const value_rounded_down = (static_cast<uint16_t>(val) >> num_bits) << num_bits;
		uint16_t const value_rounded_up =
		    (static_cast<uint16_t>(val >> num_bits) + static_cast<uint16_t>(1)) << num_bits;
		VectorRowMod16 const rounded_value = vector_round_stochastic(value, num_bits);
		for (size_t j = 0; j < rounded_value.size; ++j) {
			EXPECT_TRUE(
			    rounded_value[j] == value_rounded_down || rounded_value[j] == value_rounded_up);
		}
	}
}

TEST(VectorRoundStochastic, VectorRowFracSat16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 15;
		int const val = static_cast<int>(xorshift32(&seed) % (1ul << 16)) - (1 << 15);
		VectorRowFracSat16 value(val);
		int16_t const value_rounded_down = (static_cast<int16_t>(val) >> num_bits) << num_bits;
		int const value_rounded_up_full =
		    ((val >> num_bits) << num_bits) + (static_cast<int16_t>(1) << num_bits);
		int16_t const value_rounded_up =
		    (((value_rounded_up_full < -(1 << 15))
		          ? -(1 << 15)
		          : ((value_rounded_up_full >= (1 << 15)) ? ((1 << 15) - 1)
		                                                  : value_rounded_up_full)) >>
		     num_bits)
		    << num_bits;
		VectorRowFracSat16 const rounded_value = vector_round_stochastic(value, num_bits);
		for (size_t j = 0; j < rounded_value.size; ++j) {
			EXPECT_TRUE(
			    rounded_value[j] == value_rounded_down || rounded_value[j] == value_rounded_up);
		}
	}
}

int start()
{
	// initialize rng
	{
		uint32_t scalar_seed = 1234;
		VectorMod16 seed;
		for (size_t i = 0; i < seed.size; ++i) {
			seed[i] = xorshift32(&scalar_seed);
		}
		parallel_rng_seed(seed);
		// precondition rng storage
		for (size_t i = 0; i < 10; ++i) {
			parallel_rng_rand<VectorMod16>();
		}
	}

	return RUN_ALL_TESTS();
}
