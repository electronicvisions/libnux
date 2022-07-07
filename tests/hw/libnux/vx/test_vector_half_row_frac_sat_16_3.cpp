#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_frac_sat_16.h"


TEST(VectorHalfRowFracSat16, OperatorShiftLeftPositive)
{
	VectorHalfRowFracSat16 a(5);
	int b = 2;

	VectorHalfRowFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(20));
	EXPECT_EQ(c, VectorHalfRowFracSat16(20));

	VectorHalfRowFracSat16 d = a << b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(80));

	VectorHalfRowFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorHalfRowFracSat16(5));
}

TEST(VectorHalfRowFracSat16, OperatorShiftLeftNegative)
{
	VectorHalfRowFracSat16 a(-5);
	int b = 2;

	VectorHalfRowFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(-20));
	EXPECT_EQ(c, VectorHalfRowFracSat16(-20));

	VectorHalfRowFracSat16 d = a << b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(-80));

	VectorHalfRowFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorHalfRowFracSat16(-5));
}

TEST(VectorHalfRowFracSat16, OperatorShiftRightPositive)
{
	VectorHalfRowFracSat16 a(80);
	int b = 2;

	VectorHalfRowFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(20));
	EXPECT_EQ(c, VectorHalfRowFracSat16(20));

	VectorHalfRowFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(5));

	VectorHalfRowFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorHalfRowFracSat16(80));
}

TEST(VectorHalfRowFracSat16, OperatorShiftRightNegative)
{
	VectorHalfRowFracSat16 a(-80);
	int b = 2;

	VectorHalfRowFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(-20));
	EXPECT_EQ(c, VectorHalfRowFracSat16(-20));

	VectorHalfRowFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(-5));

	VectorHalfRowFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorHalfRowFracSat16(-80));
}

int start()
{
	return RUN_ALL_TESTS();
}
