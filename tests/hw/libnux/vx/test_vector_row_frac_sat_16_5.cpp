#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat16, OperatorShiftRightPositive)
{
	VectorRowFracSat16 a(80);
	int b = 2;

	VectorRowFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorRowFracSat16(20));
	EXPECT_EQ(c, VectorRowFracSat16(20));

	VectorRowFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorRowFracSat16(5));

	VectorRowFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorRowFracSat16(80));
}

TEST(VectorRowFracSat16, OperatorShiftRightNegative)
{
	VectorRowFracSat16 a(-80);
	int b = 2;

	VectorRowFracSat16 c = (a >>= b);
	EXPECT_EQ(a, VectorRowFracSat16(-20));
	EXPECT_EQ(c, VectorRowFracSat16(-20));

	VectorRowFracSat16 d = a >> b;
	EXPECT_EQ(d, VectorRowFracSat16(-5));

	VectorRowFracSat16 e = a >> -b;
	EXPECT_EQ(e, VectorRowFracSat16(-80));
}

int start()
{
	return RUN_ALL_TESTS();
}
