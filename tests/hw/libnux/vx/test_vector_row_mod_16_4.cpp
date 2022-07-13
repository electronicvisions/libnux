#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod16, OperatorShiftLeft)
{
	VectorRowMod16 a(10);
	int b = 2;

	VectorRowMod16 c = (a <<= b);
	EXPECT_EQ(a, VectorRowMod16(40));
	EXPECT_EQ(c, VectorRowMod16(40));

	VectorRowMod16 d = a << b;
	EXPECT_EQ(d, VectorRowMod16(160));

	VectorRowMod16 e = a << -b;
	EXPECT_EQ(e, VectorRowMod16(10));
}

TEST(VectorRowMod16, OperatorShiftRight)
{
	VectorRowMod16 a(160);
	int b = 2;

	VectorRowMod16 c = (a >>= b);
	EXPECT_EQ(a, VectorRowMod16(40));
	EXPECT_EQ(c, VectorRowMod16(40));

	VectorRowMod16 d = a >> b;
	EXPECT_EQ(d, VectorRowMod16(10));

	VectorRowMod16 e = a >> -b;
	EXPECT_EQ(e, VectorRowMod16(160));
}

int start()
{
	return RUN_ALL_TESTS();
}
