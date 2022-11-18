#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_16.h"
#include "libnux/vx/vector_if.h"
#include "libnux/vx/vector_mod_16.h"


TEST(VectorIf, VectorMod16_VectorMod16)
{
	VectorMod16 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = 3;

	VectorMod16 value_true(12);
	VectorMod16 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::lesser;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_false);
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_false[i]);
			} else {
				EXPECT_EQ(result[i], value_true[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::greater_equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_true);
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_false[i]);
			} else {
				EXPECT_EQ(result[i], value_true[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::unequal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
}

TEST(VectorIf, VectorFracSat16_VectorMod16)
{
	VectorFracSat16 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = -3;

	VectorMod16 value_true(12);
	VectorMod16 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::lesser;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == result.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_false[i]);
			} else {
				EXPECT_EQ(result[i], value_true[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::greater_equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == result.size - 1) {
				EXPECT_EQ(result[i], value_false[i]);
			} else {
				EXPECT_EQ(result[i], value_true[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0) {
				EXPECT_EQ(result[i], value_false[i]);
			} else {
				EXPECT_EQ(result[i], value_true[i]);
			}
		}
	}
	{
		auto const condition = VectorIfCondition::unequal;
		VectorMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
