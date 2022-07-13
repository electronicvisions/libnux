#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod16, OperatorMultiply)
{
	VectorRowMod16 a(30);
	VectorRowMod16 b(1000);

	VectorRowMod16 c = (a *= b);
	EXPECT_EQ(a, VectorRowMod16(30000));
	EXPECT_EQ(c, VectorRowMod16(30000));

	VectorRowMod16 d = a * b;
	EXPECT_EQ(d, VectorRowMod16(50048));

	VectorRowMod16 e = a * 1000;
	EXPECT_EQ(e, VectorRowMod16(50048));

	VectorRowMod16 f = 30000 * b;
	EXPECT_EQ(f, VectorRowMod16(50048));
}

TEST(VectorRowMod16, Conversion)
{
	VectorRowMod16 in(1000);
	EXPECT_EQ(VectorRowFracSat16(1000), static_cast<VectorRowFracSat16>(in));
}

int start()
{
	return RUN_ALL_TESTS();
}
