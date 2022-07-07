#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_16.h"


TEST(VectorFracSat16, OperatorShiftLeftPositive)
{
	VectorFracSat16 a(5);
	int b = 2;

	VectorFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorFracSat16(20));
	EXPECT_EQ(c, VectorFracSat16(20));

	VectorFracSat16 d = a << b;
	EXPECT_EQ(d, VectorFracSat16(80));

	VectorFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorFracSat16(5));
}

TEST(VectorFracSat16, OperatorShiftLeftNegative)
{
	VectorFracSat16 a(-5);
	int b = 2;

	VectorFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorFracSat16(-20));
	EXPECT_EQ(c, VectorFracSat16(-20));

	VectorFracSat16 d = a << b;
	EXPECT_EQ(d, VectorFracSat16(-80));

	VectorFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorFracSat16(-5));
}

TEST(VectorFracSat16, OperatorShiftRightPositive)
{
	VectorFracSat16 a(80);
	int b = 2;

	VectorFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorFracSat16(20));
	EXPECT_EQ(c, VectorFracSat16(20));

	VectorFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorFracSat16(5));

	VectorFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorFracSat16(80));
}

TEST(VectorFracSat16, OperatorShiftRightNegative)
{
	VectorFracSat16 a(-80);
	int b = 2;

	VectorFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorFracSat16(-20));
	EXPECT_EQ(c, VectorFracSat16(-20));

	VectorFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorFracSat16(-5));

	VectorFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorFracSat16(-80));
}

int start()
{
	return RUN_ALL_TESTS();
}
