#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_mod_16.h"


TEST(VectorHalfRowMod16, Construction)
{
	[[maybe_unused]] VectorHalfRowMod16 from_nothing;

	VectorHalfRowMod16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorHalfRowMod16::Quarter left = vec_splat_u16(42);
	VectorHalfRowMod16::Quarter right = vec_splat_u16(43);
	VectorHalfRowMod16 from_data(left, right);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i < left.size) ? left[i] : right[i - left.size]);
	}
}

TEST(VectorHalfRowMod16, OperatorUnaryminus)
{
	VectorHalfRowMod16 in(-12);
	EXPECT_EQ(-in, VectorHalfRowMod16(12));
}

TEST(VectorHalfRowMod16, OperatorPlus)
{
	VectorHalfRowMod16 a(1000);
	VectorHalfRowMod16 b(50000);

	VectorHalfRowMod16 c = (a += b);
	EXPECT_EQ(a, VectorHalfRowMod16(51000));
	EXPECT_EQ(c, VectorHalfRowMod16(51000));

	VectorHalfRowMod16 d = a + b;
	EXPECT_EQ(d, VectorHalfRowMod16(35464));

	VectorHalfRowMod16 e = a + 50000;
	EXPECT_EQ(e, VectorHalfRowMod16(35464));

	VectorHalfRowMod16 f = 51000 + b;
	EXPECT_EQ(f, VectorHalfRowMod16(35464));
}

int start()
{
	return RUN_ALL_TESTS();
}
