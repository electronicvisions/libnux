#include "libnux/vx/mailbox.h"
#include "libnux/vx/parallel_rng.h"
#include "libnux/vx/random.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/vector_round_stochastic.h"
#include "libnux/vx/vector_row.h"
#include <array>

constexpr static size_t num_tests = 10;

TEST(VectorRoundStochastic, VectorRowMod8)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 8;
		size_t const val = xorshift32(&seed) % (1ul << 8);
		VectorRowMod8 value(val);
		uint8_t const value_rounded_down = (static_cast<uint8_t>(val) >> num_bits) << num_bits;
		uint8_t const value_rounded_up =
		    (static_cast<uint8_t>(val >> num_bits) + static_cast<uint8_t>(1)) << num_bits;
		VectorRowMod8 const rounded_value = vector_round_stochastic(value, num_bits);
		for (size_t j = 0; j < rounded_value.size; ++j) {
			EXPECT_TRUE(
			    rounded_value[j] == value_rounded_down || rounded_value[j] == value_rounded_up);
		}
	}
}

TEST(VectorRoundStochastic, VectorRowFracSat8)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 7;
		int const val = static_cast<int>(xorshift32(&seed) % (1ul << 8)) - (1 << 7);
		VectorRowFracSat8 value(val);
		int8_t const value_rounded_down = (static_cast<int8_t>(val) >> num_bits) << num_bits;
		int const value_rounded_up_full =
		    ((val >> num_bits) << num_bits) + (static_cast<int8_t>(1) << num_bits);
		int8_t const value_rounded_up =
		    (((value_rounded_up_full < -(1 << 15))
		          ? -(1 << 15)
		          : ((value_rounded_up_full >= (1 << 15)) ? ((1 << 15) - 1)
		                                                  : value_rounded_up_full)) >>
		     num_bits)
		    << num_bits;
		VectorRowFracSat8 const rounded_value = vector_round_stochastic(value, num_bits);
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
		VectorMod8 seed;
		for (size_t i = 0; i < seed.size; ++i) {
			seed[i] = xorshift32(&scalar_seed);
		}
		parallel_rng_seed(seed);
		// precondition rng storage
		for (size_t i = 0; i < 10; ++i) {
			parallel_rng_rand<VectorMod8>();
		}
	}

	return RUN_ALL_TESTS();
}
