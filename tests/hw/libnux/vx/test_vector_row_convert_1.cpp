#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod8, toVectorRowFracSat8)
{
	VectorRowMod8 in(12);
	EXPECT_EQ(VectorRowFracSat8(12), static_cast<VectorRowFracSat8>(in));
}

TEST(VectorRowMod16, toVectorRowFracSat16)
{
	VectorRowMod16 in(12);
	EXPECT_EQ(VectorRowFracSat16(12), static_cast<VectorRowFracSat16>(in));
}

TEST(VectorFracSat8, toVectorRowMod8)
{
	VectorRowFracSat8 in(12);
	EXPECT_EQ(VectorRowMod8(12), static_cast<VectorRowMod8>(in));
}

TEST(VectorFracSat16, toVectorRowMod16)
{
	VectorRowFracSat16 in(12);
	EXPECT_EQ(VectorRowMod16(12), static_cast<VectorRowMod16>(in));
}

int start()
{
	return RUN_ALL_TESTS();
}
