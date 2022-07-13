#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat16, OperatorMultiply)
{
	VectorRowFracSat16 a(-3000);
	VectorRowFracSat16 b(20000);

	VectorRowFracSat16 c = (a *= b);
	EXPECT_EQ(a, VectorRowFracSat16(-1832));
	EXPECT_EQ(c, VectorRowFracSat16(-1832));

	VectorRowFracSat16 d = a * b;
	EXPECT_EQ(d, VectorRowFracSat16(-1119));

	VectorRowFracSat16 e = a * 20000;
	EXPECT_EQ(e, VectorRowFracSat16(-1119));

	VectorRowFracSat16 f = -1832 * b;
	EXPECT_EQ(f, VectorRowFracSat16(-1119));
}

TEST(VectorRowFracSat16, Conversion)
{
	VectorRowFracSat16 in(-12);
	EXPECT_EQ(VectorRowMod16(-12), static_cast<VectorRowMod16>(in));
}

int start()
{
	return RUN_ALL_TESTS();
}
