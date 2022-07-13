#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat8, OperatorPlus)
{
	VectorRowFracSat8 a(-12);
	VectorRowFracSat8 b(80);

	VectorRowFracSat8 c = (a += b);
	EXPECT_EQ(a, VectorRowFracSat8(68));
	EXPECT_EQ(c, VectorRowFracSat8(68));

	VectorRowFracSat8 d = a + b;
	EXPECT_EQ(d, VectorRowFracSat8(127));

	VectorRowFracSat8 e = a + 80;
	EXPECT_EQ(e, VectorRowFracSat8(127));

	VectorRowFracSat8 f = 68 + b;
	EXPECT_EQ(f, VectorRowFracSat8(127));
}

TEST(VectorRowFracSat8, OperatorMinus)
{
	VectorRowFracSat8 a(-12);
	VectorRowFracSat8 b(60);

	VectorRowFracSat8 c = (a -= b);
	EXPECT_EQ(a, VectorRowFracSat8(-72));
	EXPECT_EQ(c, VectorRowFracSat8(-72));

	VectorRowFracSat8 d = a - b;
	EXPECT_EQ(d, VectorRowFracSat8(-128));

	VectorRowFracSat8 e = a - 60;
	EXPECT_EQ(e, VectorRowFracSat8(-128));

	VectorRowFracSat8 f = -72 - b;
	EXPECT_EQ(f, VectorRowFracSat8(-128));
}

TEST(VectorRowFracSat8, OperatorMultiply)
{
	VectorRowFracSat8 a(-12);
	VectorRowFracSat8 b(42);

	VectorRowFracSat8 c = (a *= b);
	EXPECT_EQ(a, VectorRowFracSat8(-4));
	EXPECT_EQ(c, VectorRowFracSat8(-4));

	VectorRowFracSat8 d = a * b;
	EXPECT_EQ(d, VectorRowFracSat8(-2));

	VectorRowFracSat8 e = a * 42;
	EXPECT_EQ(e, VectorRowFracSat8(-2));

	VectorRowFracSat8 f = -4 * b;
	EXPECT_EQ(f, VectorRowFracSat8(-2));
}

TEST(VectorRowFracSat8, Conversion)
{
	VectorRowFracSat8 in(-12);
	EXPECT_EQ(VectorRowMod8(-12), static_cast<VectorRowMod8>(in));
}

int start()
{
	return RUN_ALL_TESTS();
}
