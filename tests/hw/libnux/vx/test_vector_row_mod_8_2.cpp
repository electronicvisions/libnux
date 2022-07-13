#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod8, OperatorMinus)
{
	VectorRowMod8 a(140);
	VectorRowMod8 b(130);

	VectorRowMod8 c = (a -= b);
	EXPECT_EQ(a, VectorRowMod8(10));
	EXPECT_EQ(c, VectorRowMod8(10));

	VectorRowMod8 d = a - b;
	EXPECT_EQ(d, VectorRowMod8(136));

	VectorRowMod8 e = a - 130;
	EXPECT_EQ(e, VectorRowMod8(136));

	VectorRowMod8 f = 10 - b;
	EXPECT_EQ(f, VectorRowMod8(136));
}

TEST(VectorRowMod8, OperatorMultiply)
{
	VectorRowMod8 a(10);
	VectorRowMod8 b(12);

	VectorRowMod8 c = (a *= b);
	EXPECT_EQ(a, VectorRowMod8(120));
	EXPECT_EQ(c, VectorRowMod8(120));

	VectorRowMod8 d = a * b;
	EXPECT_EQ(d, VectorRowMod8(160));

	VectorRowMod8 e = a * 12;
	EXPECT_EQ(e, VectorRowMod8(160));

	VectorRowMod8 f = 120 * b;
	EXPECT_EQ(f, VectorRowMod8(160));
}

TEST(VectorRowMod8, Conversion)
{
	VectorRowMod8 in(12);
	EXPECT_EQ(VectorRowFracSat8(12), static_cast<VectorRowFracSat8>(in));
}

int start()
{
	return RUN_ALL_TESTS();
}
