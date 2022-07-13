#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod8, OperatorShiftLeft)
{
	VectorRowMod8 a(10);
	int b = 2;

	VectorRowMod8 c = (a <<= b);
	EXPECT_EQ(a, VectorRowMod8(40));
	EXPECT_EQ(c, VectorRowMod8(40));

	VectorRowMod8 d = a << b;
	EXPECT_EQ(d, VectorRowMod8(160));

	VectorRowMod8 e = a << -b;
	EXPECT_EQ(e, VectorRowMod8(10));
}

TEST(VectorRowMod8, OperatorShiftRight)
{
	VectorRowMod8 a(160);
	int b = 2;

	VectorRowMod8 c = (a >>= b);
	EXPECT_EQ(a, VectorRowMod8(40));
	EXPECT_EQ(c, VectorRowMod8(40));

	VectorRowMod8 d = a >> b;
	EXPECT_EQ(d, VectorRowMod8(10));

	VectorRowMod8 e = a >> -b;
	EXPECT_EQ(e, VectorRowMod8(160));
}

int start()
{
	return RUN_ALL_TESTS();
}
