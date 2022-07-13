#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod8, convert_contiguous)
{
	VectorRowMod8 in(12);
	EXPECT_EQ(VectorRowFracSat8(-116), in.convert_contiguous());
}

TEST(VectorRowMod16, convert_contiguous)
{
	VectorRowMod16 in(12);
	EXPECT_EQ(VectorRowFracSat16(-32756), in.convert_contiguous());
}

TEST(VectorFracSat8, convert_contiguous)
{
	VectorRowFracSat8 in(-116);
	EXPECT_EQ(VectorRowMod8(12), in.convert_contiguous());
}

TEST(VectorFracSat16, convert_contiguous)
{
	VectorRowFracSat16 in(-32756);
	EXPECT_EQ(VectorRowMod16(12), in.convert_contiguous());
}

int start()
{
	return RUN_ALL_TESTS();
}
