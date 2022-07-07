#include "libnux/vx/testing.h"
#include "libnux/vx/vector_mod_8.h"


TEST(VectorMod8, Construction)
{
	[[maybe_unused]] VectorMod8 from_nothing;

	VectorMod8 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorMod8::Data data = vec_splat_u8(42);
	VectorMod8 from_data(data);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], data[i]);
	}
}

TEST(VectorMod8, OperatorUnaryminus)
{
	VectorMod8 in(-12);
	EXPECT_EQ(-in, VectorMod8(12));
}

TEST(VectorMod8, OperatorPlus)
{
	VectorMod8 a(10);
	VectorMod8 b(140);

	VectorMod8 c = (a += b);
	EXPECT_EQ(a, VectorMod8(150));
	EXPECT_EQ(c, VectorMod8(150));

	VectorMod8 d = a + b;
	EXPECT_EQ(d, VectorMod8(34));

	VectorMod8 e = a + 140;
	EXPECT_EQ(e, VectorMod8(34));

	VectorMod8 f = 150 + b;
	EXPECT_EQ(f, VectorMod8(34));
}

TEST(VectorMod8, OperatorMinus)
{
	VectorMod8 a(140);
	VectorMod8 b(130);

	VectorMod8 c = (a -= b);
	EXPECT_EQ(a, VectorMod8(10));
	EXPECT_EQ(c, VectorMod8(10));

	VectorMod8 d = a - b;
	EXPECT_EQ(d, VectorMod8(136));

	VectorMod8 e = a - 130;
	EXPECT_EQ(e, VectorMod8(136));

	VectorMod8 f = 10 - b;
	EXPECT_EQ(f, VectorMod8(136));
}

TEST(VectorMod8, OperatorMultiply)
{
	VectorMod8 a(10);
	VectorMod8 b(12);

	VectorMod8 c = (a *= b);
	EXPECT_EQ(a, VectorMod8(120));
	EXPECT_EQ(c, VectorMod8(120));

	VectorMod8 d = a * b;
	EXPECT_EQ(d, VectorMod8(160));

	VectorMod8 e = a * 12;
	EXPECT_EQ(e, VectorMod8(160));

	VectorMod8 f = 120 * b;
	EXPECT_EQ(f, VectorMod8(160));
}

TEST(VectorMod8, Conversion)
{
	{
		VectorMod8 in(12);
		EXPECT_EQ(VectorFracSat8(-116), in.convert_contiguous());
	}

	{
		VectorMod8 in(12);
		EXPECT_EQ(VectorFracSat8(12), static_cast<VectorFracSat8>(in));
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
