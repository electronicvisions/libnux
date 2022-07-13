#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod16, OperatorMinus)
{
	VectorRowMod16 a(40000);
	VectorRowMod16 b(21000);

	VectorRowMod16 c = (a -= b);
	EXPECT_EQ(a, VectorRowMod16(19000));
	EXPECT_EQ(c, VectorRowMod16(19000));

	VectorRowMod16 d = a - b;
	EXPECT_EQ(d, VectorRowMod16(63536));

	VectorRowMod16 e = a - 21000;
	EXPECT_EQ(e, VectorRowMod16(63536));

	VectorRowMod16 f = 19000 - b;
	EXPECT_EQ(f, VectorRowMod16(63536));
}

int start()
{
	return RUN_ALL_TESTS();
}
