#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_mod_16.h"
#include "libnux/vx/vector_if.h"
#include "libnux/vx/vector_row.h"


TEST(VectorIf, VectorHalfRowMod16_VectorHalfRowMod16)
{
	VectorHalfRowMod16 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = 3;

	VectorHalfRowMod16 value_true(12);
	VectorHalfRowMod16 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_false);
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);

		EXPECT_EQ(result, value_true);
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorHalfRowMod16 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
}

TEST(VectorIf, VectorRowMod16_VectorRowMod16)
{
	VectorRowMod16 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = 3;

	VectorRowMod16 value_true(12);
	VectorRowMod16 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);

		EXPECT_EQ(result, value_false);
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_true);
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod16 const result = vector_if(mask, condition, value_true, value_false);

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
