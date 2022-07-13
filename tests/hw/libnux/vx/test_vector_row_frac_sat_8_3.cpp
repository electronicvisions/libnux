#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat8, OperatorShiftLeftPositive)
{
	VectorRowFracSat8 a(5);
	int b = 2;

	VectorRowFracSat8 c = (a <<= b);
	EXPECT_EQ(a, VectorRowFracSat8(20));
	EXPECT_EQ(c, VectorRowFracSat8(20));

	VectorRowFracSat8 d = a << b;
	EXPECT_EQ(d, VectorRowFracSat8(80));

	VectorRowFracSat8 e = a << -b;
	EXPECT_EQ(e, VectorRowFracSat8(5));
}

TEST(VectorRowFracSat8, OperatorShiftLeftNegative)
{
	VectorRowFracSat8 a(-5);
	int b = 2;

	VectorRowFracSat8 c = (a <<= b);
	EXPECT_EQ(a, VectorRowFracSat8(-20));
	EXPECT_EQ(c, VectorRowFracSat8(-20));

	VectorRowFracSat8 d = a << b;
	EXPECT_EQ(d, VectorRowFracSat8(-80));

	VectorRowFracSat8 e = a << -b;
	EXPECT_EQ(e, VectorRowFracSat8(-5));
}

TEST(VectorRowFracSat8, OperatorShiftRightPositive)
{
	VectorRowFracSat8 a(80);
	int b = 2;

	VectorRowFracSat8 c = (a >>= b);
	EXPECT_EQ(a, VectorRowFracSat8(20));
	EXPECT_EQ(c, VectorRowFracSat8(20));

	VectorRowFracSat8 d = a >> b;
	EXPECT_EQ(d, VectorRowFracSat8(5));

	VectorRowFracSat8 e = a >> -b;
	EXPECT_EQ(e, VectorRowFracSat8(80));
}

TEST(VectorRowFracSat8, OperatorShiftRightNegative)
{
	VectorRowFracSat8 a(-80);
	int b = 2;

	VectorRowFracSat8 c = (a >>= b);
	EXPECT_EQ(a, VectorRowFracSat8(-20));
	EXPECT_EQ(c, VectorRowFracSat8(-20));

	VectorRowFracSat8 d = a >> b;
	EXPECT_EQ(d, VectorRowFracSat8(-5));

	VectorRowFracSat8 e = a >> -b;
	EXPECT_EQ(e, VectorRowFracSat8(-80));
}

int start()
{
	return RUN_ALL_TESTS();
}
