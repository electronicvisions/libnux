#include "libnux/vx/mailbox.h"
#include "libnux/vx/parallel_rng.h"
#include "libnux/vx/random.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_16.h"
#include "libnux/vx/vector_mod_16.h"
#include "libnux/vx/vector_round_stochastic.h"
#include <array>

constexpr static size_t num_tests = 10;
constexpr static size_t num = 10;

TEST(VectorRoundStochastic, VectorMod16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 16;
		size_t const val = xorshift32(&seed) % (1ul << 16);
		VectorMod16 value(val);
		uint16_t const value_rounded_down = (static_cast<uint16_t>(val) >> num_bits) << num_bits;
		uint16_t const value_rounded_up =
		    (static_cast<uint16_t>(val >> num_bits) + static_cast<uint16_t>(1)) << num_bits;
		size_t num_rounded_up = 0;
		for (size_t i = 0; i < num; ++i) {
			VectorMod16 const rounded_value = vector_round_stochastic(value, num_bits);
			if (i == 0) {
				for (size_t j = 0; j < rounded_value.size; ++j) {
					EXPECT_TRUE(
					    rounded_value[j] == value_rounded_down ||
					    rounded_value[j] == value_rounded_up);
				}
			}
			for (size_t j = 0; j < rounded_value.size; ++j) {
				bool const is_rounded_up = (rounded_value[j] == value_rounded_up);
				num_rounded_up += is_rounded_up;
			}
		}
		// expect (val & fill(num_bits)) / (1 << num_bits) probability
		float const scale = static_cast<float>(num * value.size) / (1 << num_bits);
		float const mid = val & ((1 << num_bits) - 1);
		// allowed deviation of expectation >= 1 means bit-correctness is not validated, but is
		// required for reasonable test duration, because sharper results need more samples
		float const max_dev = 2000;
		EXPECT_GT(num_rounded_up, scale * (mid - max_dev));
		EXPECT_LT(num_rounded_up, scale * (mid + max_dev));
	}
}

TEST(VectorRoundStochastic, VectorFracSat16)
{
	uint32_t seed = 1234;
	for (size_t n = 0; n < num_tests; ++n) {
		size_t const num_bits = xorshift32(&seed) % 15;
		int const val = static_cast<int>(xorshift32(&seed) % (1ul << 16)) - (1 << 15);
		VectorFracSat16 value(val);
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
		size_t num_rounded_up = 0;
		for (size_t i = 0; i < num; ++i) {
			VectorFracSat16 const rounded_value = vector_round_stochastic(value, num_bits);
			if (i == 0) {
				for (size_t j = 0; j < rounded_value.size; ++j) {
					EXPECT_TRUE(
					    rounded_value[j] == value_rounded_down ||
					    rounded_value[j] == value_rounded_up);
				}
			}
			for (size_t j = 0; j < rounded_value.size; ++j) {
				bool const is_rounded_up = (rounded_value[j] == value_rounded_up);
				num_rounded_up += is_rounded_up;
			}
		}
		if (value_rounded_up != value_rounded_down) {
			// expect (val & fill(num_bits)) / (1 << num_bits) probability
			float const scale = static_cast<float>(num * value.size) / (1 << num_bits);
			float const mid =
			    static_cast<uint16_t>(static_cast<int16_t>(val)) & ((1 << num_bits) - 1);
			// allowed deviation of expectation >= 1 means bit-correctness is not validated, but is
			// required for reasonable test duration, because sharper results need more samples
			float const max_dev = 200;
			EXPECT_GT(num_rounded_up, scale * (mid - max_dev));
			EXPECT_LT(num_rounded_up, scale * (mid + max_dev));
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
