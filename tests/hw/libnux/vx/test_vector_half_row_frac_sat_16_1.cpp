#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_frac_sat_16.h"


TEST(VectorHalfRowFracSat16, Construction)
{
	[[maybe_unused]] VectorHalfRowFracSat16 from_nothing;

	VectorHalfRowFracSat16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorHalfRowFracSat16::Quarter left = vec_splat_s16(42);
	VectorHalfRowFracSat16::Quarter right = vec_splat_s16(43);
	VectorHalfRowFracSat16 from_data(left, right);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i < left.size) ? left[i] : right[i - left.size]);
	}
}

TEST(VectorHalfRowFracSat16, OperatorUnaryminus)
{
	VectorHalfRowFracSat16 in(-12);
	EXPECT_EQ(-in, VectorHalfRowFracSat16(12));
}

TEST(VectorHalfRowFracSat16, OperatorPlus)
{
	VectorHalfRowFracSat16 a(-3000);
	VectorHalfRowFracSat16 b(20000);

	VectorHalfRowFracSat16 c = (a += b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(17000));
	EXPECT_EQ(c, VectorHalfRowFracSat16(17000));

	VectorHalfRowFracSat16 d = a + b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(32767));

	VectorHalfRowFracSat16 e = a + 20000;
	EXPECT_EQ(e, VectorHalfRowFracSat16(32767));

	VectorHalfRowFracSat16 f = 17000 + b;
	EXPECT_EQ(f, VectorHalfRowFracSat16(32767));
}

int start()
{
	return RUN_ALL_TESTS();
}
