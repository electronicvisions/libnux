#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_mod_16.h"


TEST(VectorHalfRowMod16, OperatorShiftLeft)
{
	VectorHalfRowMod16 a(10);
	int b = 2;

	VectorHalfRowMod16 c = (a <<= b);
	EXPECT_EQ(a, VectorHalfRowMod16(40));
	EXPECT_EQ(c, VectorHalfRowMod16(40));

	VectorHalfRowMod16 d = a << b;
	EXPECT_EQ(d, VectorHalfRowMod16(160));

	VectorHalfRowMod16 e = a << -b;
	EXPECT_EQ(e, VectorHalfRowMod16(10));
}

TEST(VectorHalfRowMod16, OperatorShiftRight)
{
	VectorHalfRowMod16 a(160);
	int b = 2;

	VectorHalfRowMod16 c = (a >>= b);
	EXPECT_EQ(a, VectorHalfRowMod16(40));
	EXPECT_EQ(c, VectorHalfRowMod16(40));

	VectorHalfRowMod16 d = a >> b;
	EXPECT_EQ(d, VectorHalfRowMod16(10));

	VectorHalfRowMod16 e = a >> -b;
	EXPECT_EQ(e, VectorHalfRowMod16(160));
}

int start()
{
	return RUN_ALL_TESTS();
}
