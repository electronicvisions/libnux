#include "libnux/vx/testing.h"
#include "libnux/vx/vector_half_row_mod_16.h"


TEST(VectorHalfRowMod16, OperatorMinus)
{
	VectorHalfRowMod16 a(40000);
	VectorHalfRowMod16 b(21000);

	VectorHalfRowMod16 c = (a -= b);
	EXPECT_EQ(a, VectorHalfRowMod16(19000));
	EXPECT_EQ(c, VectorHalfRowMod16(19000));

	VectorHalfRowMod16 d = a - b;
	EXPECT_EQ(d, VectorHalfRowMod16(63536));

	VectorHalfRowMod16 e = a - 21000;
	EXPECT_EQ(e, VectorHalfRowMod16(63536));

	VectorHalfRowMod16 f = 19000 - b;
	EXPECT_EQ(f, VectorHalfRowMod16(63536));
}

TEST(VectorHalfRowMod16, OperatorMultiply)
{
	VectorHalfRowMod16 a(30);
	VectorHalfRowMod16 b(1000);

	VectorHalfRowMod16 c = (a *= b);
	EXPECT_EQ(a, VectorHalfRowMod16(30000));
	EXPECT_EQ(c, VectorHalfRowMod16(30000));

	VectorHalfRowMod16 d = a * b;
	EXPECT_EQ(d, VectorHalfRowMod16(50048));

	VectorHalfRowMod16 e = a * 1000;
	EXPECT_EQ(e, VectorHalfRowMod16(50048));

	VectorHalfRowMod16 f = 30000 * b;
	EXPECT_EQ(f, VectorHalfRowMod16(50048));
}

TEST(VectorHalfRowMod16, Conversion)
{
	{
		VectorHalfRowMod16 in(1000);
		EXPECT_EQ(VectorHalfRowFracSat16(-31768), in.convert_contiguous());
	}
	{
		VectorHalfRowMod16 in(1000);
		EXPECT_EQ(VectorHalfRowFracSat16(1000), static_cast<VectorHalfRowFracSat16>(in));
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
