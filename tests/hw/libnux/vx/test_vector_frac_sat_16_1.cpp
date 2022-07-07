#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_16.h"


TEST(VectorFracSat16, Construction)
{
	[[maybe_unused]] VectorFracSat16 from_nothing;

	VectorFracSat16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorFracSat16::Data data = vec_splat_s16(42);
	VectorFracSat16 from_data(data);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], data[i]);
	}
}

TEST(VectorFracSat16, OperatorUnaryminus)
{
	VectorFracSat16 in(-12);
	EXPECT_EQ(-in, VectorFracSat16(12));
}

TEST(VectorFracSat16, OperatorPlus)
{
	VectorFracSat16 a(-3000);
	VectorFracSat16 b(20000);

	VectorFracSat16 c = (a += b);
	EXPECT_EQ(a, VectorFracSat16(17000));
	EXPECT_EQ(c, VectorFracSat16(17000));

	VectorFracSat16 d = a + b;
	EXPECT_EQ(d, VectorFracSat16(32767));

	VectorFracSat16 e = a + 20000;
	EXPECT_EQ(e, VectorFracSat16(32767));

	VectorFracSat16 f = 17000 + b;
	EXPECT_EQ(f, VectorFracSat16(32767));
}

TEST(VectorFracSat16, OperatorMinus)
{
	VectorFracSat16 a(-3000);
	VectorFracSat16 b(20000);

	VectorFracSat16 c = (a -= b);
	EXPECT_EQ(a, VectorFracSat16(-23000));
	EXPECT_EQ(c, VectorFracSat16(-23000));

	VectorFracSat16 d = a - b;
	EXPECT_EQ(d, VectorFracSat16(-32768));

	VectorFracSat16 e = a - 20000;
	EXPECT_EQ(e, VectorFracSat16(-32768));

	VectorFracSat16 f = -23000 - b;
	EXPECT_EQ(f, VectorFracSat16(-32768));
}

TEST(VectorFracSat16, OperatorMultiply)
{
	VectorFracSat16 a(-3000);
	VectorFracSat16 b(20000);

	VectorFracSat16 c = (a *= b);
	EXPECT_EQ(a, VectorFracSat16(-1832));
	EXPECT_EQ(c, VectorFracSat16(-1832));

	VectorFracSat16 d = a * b;
	EXPECT_EQ(d, VectorFracSat16(-1119));

	VectorFracSat16 e = a * 20000;
	EXPECT_EQ(e, VectorFracSat16(-1119));

	VectorFracSat16 f = -1832 * b;
	EXPECT_EQ(f, VectorFracSat16(-1119));
}

TEST(VectorFracSat16, Conversion)
{
	{
		VectorFracSat16 in(-12);
		EXPECT_EQ(VectorMod16(32756), in.convert_contiguous());
	}

	{
		VectorFracSat16 in(-12);
		EXPECT_EQ(VectorMod16(-12), static_cast<VectorMod16>(in));
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
