#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_8.h"


TEST(VectorFracSat8, OperatorShiftLeftPositive)
{
	VectorFracSat8 a(5);
	int b = 2;

	VectorFracSat8 c = (a <<= b);
	EXPECT_EQ(a, VectorFracSat8(20));
	EXPECT_EQ(c, VectorFracSat8(20));

	VectorFracSat8 d = a << b;
	EXPECT_EQ(d, VectorFracSat8(80));

	VectorFracSat8 e = a << -b;
	EXPECT_EQ(e, VectorFracSat8(5));
}

TEST(VectorFracSat8, OperatorShiftLeftNegative)
{
	VectorFracSat8 a(-5);
	int b = 2;

	VectorFracSat8 c = (a <<= b);
	EXPECT_EQ(a, VectorFracSat8(-20));
	EXPECT_EQ(c, VectorFracSat8(-20));

	VectorFracSat8 d = a << b;
	EXPECT_EQ(d, VectorFracSat8(-80));

	VectorFracSat8 e = a << -b;
	EXPECT_EQ(e, VectorFracSat8(-5));
}

TEST(VectorFracSat8, OperatorShiftRightPositive)
{
	VectorFracSat8 a(80);
	int b = 2;

	VectorFracSat8 c = (a >>= b);
	EXPECT_EQ(a, VectorFracSat8(20));
	EXPECT_EQ(c, VectorFracSat8(20));

	VectorFracSat8 d = a >> b;
	EXPECT_EQ(d, VectorFracSat8(5));

	VectorFracSat8 e = a >> -b;
	EXPECT_EQ(e, VectorFracSat8(80));
}

TEST(VectorFracSat8, OperatorShiftRightNegative)
{
	VectorFracSat8 a(-80);
	int b = 2;

	VectorFracSat8 c = (a >>= b);
	EXPECT_EQ(a, VectorFracSat8(-20));
	EXPECT_EQ(c, VectorFracSat8(-20));

	VectorFracSat8 d = a >> b;
	EXPECT_EQ(d, VectorFracSat8(-5));

	VectorFracSat8 e = a >> -b;
	EXPECT_EQ(e, VectorFracSat8(-80));
}

int start()
{
	return RUN_ALL_TESTS();
}
