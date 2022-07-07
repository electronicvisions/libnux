#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_frac_sat_16.h"


TEST(VectorHalfRowFracSat16, OperatorMinus)
{
	VectorHalfRowFracSat16 a(-3000);
	VectorHalfRowFracSat16 b(20000);

	VectorHalfRowFracSat16 c = (a -= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(-23000));
	EXPECT_EQ(c, VectorHalfRowFracSat16(-23000));

	VectorHalfRowFracSat16 d = a - b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(-32768));

	VectorHalfRowFracSat16 e = a - 20000;
	EXPECT_EQ(e, VectorHalfRowFracSat16(-32768));

	VectorHalfRowFracSat16 f = -23000 - b;
	EXPECT_EQ(f, VectorHalfRowFracSat16(-32768));
}

TEST(VectorHalfRowFracSat16, OperatorMultiply)
{
	VectorHalfRowFracSat16 a(-3000);
	VectorHalfRowFracSat16 b(20000);

	VectorHalfRowFracSat16 c = (a *= b);
	EXPECT_EQ(a, VectorHalfRowFracSat16(-1832));
	EXPECT_EQ(c, VectorHalfRowFracSat16(-1832));

	VectorHalfRowFracSat16 d = a * b;
	EXPECT_EQ(d, VectorHalfRowFracSat16(-1119));

	VectorHalfRowFracSat16 e = a * 20000;
	EXPECT_EQ(e, VectorHalfRowFracSat16(-1119));

	VectorHalfRowFracSat16 f = -1832 * b;
	EXPECT_EQ(f, VectorHalfRowFracSat16(-1119));
}

TEST(VectorHalfRowFracSat16, Conversion)
{
	{
		VectorHalfRowFracSat16 in(-12);
		EXPECT_EQ(VectorHalfRowMod16(-12), static_cast<VectorHalfRowMod16>(in));
	}

	{
		VectorHalfRowFracSat16 in(-12);
		EXPECT_EQ(VectorHalfRowMod16(32756), in.convert_contiguous());
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
