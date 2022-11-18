#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_8.h"
#include "libnux/vx/vector_if.h"
#include "libnux/vx/vector_mod_8.h"
#include "libnux/vx/vector_row.h"


TEST(VectorIf, VectorMod8_VectorMod8)
{
	VectorMod8 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = 3;

	VectorMod8 value_true(12);
	VectorMod8 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_false);
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_true);
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
}

TEST(VectorIf, VectorFracSat8_VectorMod8)
{
	VectorFracSat8 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = -3;

	VectorMod8 value_true(12);
	VectorMod8 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorMod8 const result = vector_if(mask, condition, value_true, value_false);

		for (size_t i = 0; i < result.size; ++i) {
			if (i == 0 || i == mask.size - 1) {
				EXPECT_EQ(result[i], value_true[i]);
			} else {
				EXPECT_EQ(result[i], value_false[i]);
			}
		}
	}
}

TEST(VectorIf, VectorRowMod8_VectorRowMod8)
{
	VectorRowMod8 mask(0);
	mask[0] = 1;
	mask[mask.size - 1] = 3;

	VectorRowMod8 value_true(12);
	VectorRowMod8 value_false(42);

	{
		auto const condition = VectorIfCondition::greater;
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_false);
	}
	{
		auto const condition = VectorIfCondition::equal;
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);
		EXPECT_EQ(result, value_true);
	}
	{
		auto const condition = VectorIfCondition::lesser_equal;
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);

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
		VectorRowMod8 const result = vector_if(mask, condition, value_true, value_false);

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
