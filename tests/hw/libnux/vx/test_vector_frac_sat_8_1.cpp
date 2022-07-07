#include "libnux/vx/testing.h"
#include "libnux/vx/vector_frac_sat_8.h"


TEST(VectorFracSat8, Construction)
{
	[[maybe_unused]] VectorFracSat8 from_nothing;

	VectorFracSat8 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorFracSat8::Data data = vec_splat_s8(42);
	VectorFracSat8 from_data(data);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], data[i]);
	}
}

TEST(VectorFracSat8, OperatorUnaryminus)
{
	VectorFracSat8 in(-12);
	EXPECT_EQ(-in, VectorFracSat8(12));
}

TEST(VectorFracSat8, OperatorPlus)
{
	VectorFracSat8 a(-12);
	VectorFracSat8 b(80);

	VectorFracSat8 c = (a += b);
	EXPECT_EQ(a, VectorFracSat8(68));
	EXPECT_EQ(c, VectorFracSat8(68));

	VectorFracSat8 d = a + b;
	EXPECT_EQ(d, VectorFracSat8(127));

	VectorFracSat8 e = a + 80;
	EXPECT_EQ(e, VectorFracSat8(127));

	VectorFracSat8 f = 68 + b;
	EXPECT_EQ(f, VectorFracSat8(127));
}

TEST(VectorFracSat8, OperatorMinus)
{
	VectorFracSat8 a(-12);
	VectorFracSat8 b(60);

	VectorFracSat8 c = (a -= b);
	EXPECT_EQ(a, VectorFracSat8(-72));
	EXPECT_EQ(c, VectorFracSat8(-72));

	VectorFracSat8 d = a - b;
	EXPECT_EQ(d, VectorFracSat8(-128));

	VectorFracSat8 e = a - 60;
	EXPECT_EQ(e, VectorFracSat8(-128));

	VectorFracSat8 f = -72 - b;
	EXPECT_EQ(f, VectorFracSat8(-128));
}

TEST(VectorFracSat8, OperatorMultiply)
{
	VectorFracSat8 a(-12);
	VectorFracSat8 b(42);

	VectorFracSat8 c = (a *= b);
	EXPECT_EQ(a, VectorFracSat8(-4));
	EXPECT_EQ(c, VectorFracSat8(-4));

	VectorFracSat8 d = a * b;
	EXPECT_EQ(d, VectorFracSat8(-2));

	VectorFracSat8 e = a * 42;
	EXPECT_EQ(e, VectorFracSat8(-2));

	VectorFracSat8 f = -4 * b;
	EXPECT_EQ(f, VectorFracSat8(-2));
}

TEST(VectorFracSat8, Conversion)
{
	{
		VectorFracSat8 in(-12);
		EXPECT_EQ(VectorMod8(116), in.convert_contiguous());
	}

	{
		VectorFracSat8 in(-12);
		EXPECT_EQ(VectorMod8(-12), static_cast<VectorMod8>(in));
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
