#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row_math.h"
#include "libnux/vx/vector_row.h"

TEST(pow, General)
{
	libnux::vx::VectorRowMod8 base(3);

	{
		size_t const exponent = 0;
		auto const result = libnux::vx::pow(base, exponent);

		bool matches_expectation = true;
		for (size_t i = 0; i < result.size; ++i) {
			matches_expectation = matches_expectation && (result[i] == 1);
		}
		EXPECT_TRUE(matches_expectation);
	}

	{
		size_t const exponent = 1;
		auto const result = libnux::vx::pow(base, exponent);

		bool matches_expectation = true;
		for (size_t i = 0; i < result.size; ++i) {
			matches_expectation = matches_expectation && (result[i] == base[i]);
		}
		EXPECT_TRUE(matches_expectation);
	}

	{
		size_t const exponent = 3;
		auto const result = libnux::vx::pow(base, exponent);

		bool matches_expectation = true;
		for (size_t i = 0; i < result.size; ++i) {
			matches_expectation = matches_expectation && (result[i] == base[i] * base[i] * base[i]);
		}
		EXPECT_TRUE(matches_expectation);
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
