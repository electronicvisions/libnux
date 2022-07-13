#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat16, OperatorShiftLeftPositive)
{
	VectorRowFracSat16 a(5);
	int b = 2;

	VectorRowFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorRowFracSat16(20));
	EXPECT_EQ(c, VectorRowFracSat16(20));

	VectorRowFracSat16 d = a << b;
	EXPECT_EQ(d, VectorRowFracSat16(80));

	VectorRowFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorRowFracSat16(5));
}

TEST(VectorRowFracSat16, OperatorShiftLeftNegative)
{
	VectorRowFracSat16 a(-5);
	int b = 2;

	VectorRowFracSat16 c = (a <<= b);
	EXPECT_EQ(a, VectorRowFracSat16(-20));
	EXPECT_EQ(c, VectorRowFracSat16(-20));

	VectorRowFracSat16 d = a << b;
	EXPECT_EQ(d, VectorRowFracSat16(-80));

	VectorRowFracSat16 e = a << -b;
	EXPECT_EQ(e, VectorRowFracSat16(-5));
}

int start()
{
	return RUN_ALL_TESTS();
}
