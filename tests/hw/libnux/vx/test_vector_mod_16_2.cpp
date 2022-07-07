#include "libnux/vx/testing.h"
#include "libnux/vx/vector_mod_16.h"


TEST(VectorMod16, OperatorShiftLeft)
{
	VectorMod16 a(10);
	int b = 2;

	VectorMod16 c = (a <<= b);
	EXPECT_EQ(a, VectorMod16(40));
	EXPECT_EQ(c, VectorMod16(40));

	VectorMod16 d = a << b;
	EXPECT_EQ(d, VectorMod16(160));

	VectorMod16 e = a << -b;
	EXPECT_EQ(e, VectorMod16(10));
}

TEST(VectorMod16, OperatorShiftRight)
{
	VectorMod16 a(160);
	int b = 2;

	VectorMod16 c = (a >>= b);
	EXPECT_EQ(a, VectorMod16(40));
	EXPECT_EQ(c, VectorMod16(40));

	VectorMod16 d = a >> b;
	EXPECT_EQ(d, VectorMod16(10));

	VectorMod16 e = a >> -b;
	EXPECT_EQ(e, VectorMod16(160));
}

int start()
{
	return RUN_ALL_TESTS();
}
