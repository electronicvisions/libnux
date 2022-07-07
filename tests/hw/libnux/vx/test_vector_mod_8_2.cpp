#include "libnux/vx/testing.h"
#include "libnux/vx/vector_mod_8.h"


TEST(VectorMod8, OperatorShiftLeft)
{
	VectorMod8 a(10);
	int b = 2;

	VectorMod8 c = (a <<= b);
	EXPECT_EQ(a, VectorMod8(40));
	EXPECT_EQ(c, VectorMod8(40));

	VectorMod8 d = a << b;
	EXPECT_EQ(d, VectorMod8(160));

	VectorMod8 e = a << -b;
	EXPECT_EQ(e, VectorMod8(10));
}

TEST(VectorMod8, OperatorShiftRight)
{
	VectorMod8 a(160);
	int b = 2;

	VectorMod8 c = (a >>= b);
	EXPECT_EQ(a, VectorMod8(40));
	EXPECT_EQ(c, VectorMod8(40));

	VectorMod8 d = a >> b;
	EXPECT_EQ(d, VectorMod8(10));

	VectorMod8 e = a >> -b;
	EXPECT_EQ(e, VectorMod8(160));
}

int start()
{
	return RUN_ALL_TESTS();
}
