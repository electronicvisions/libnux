#include "libnux/vx/testing.h"
#include "libnux/vx/vector_mod_16.h"


TEST(VectorMod16, Construction)
{
	[[maybe_unused]] VectorMod16 from_nothing;

	VectorMod16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorMod16::Data data = vec_splat_u16(42);
	VectorMod16 from_data(data);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], data[i]);
	}
}

TEST(VectorMod16, OperatorUnaryminus)
{
	VectorMod16 in(-12);
	EXPECT_EQ(-in, VectorMod16(12));
}

TEST(VectorMod16, OperatorPlus)
{
	VectorMod16 a(1000);
	VectorMod16 b(50000);

	VectorMod16 c = (a += b);
	EXPECT_EQ(a, VectorMod16(51000));
	EXPECT_EQ(c, VectorMod16(51000));

	VectorMod16 d = a + b;
	EXPECT_EQ(d, VectorMod16(35464));

	VectorMod16 e = a + 50000;
	EXPECT_EQ(e, VectorMod16(35464));

	VectorMod16 f = 51000 + b;
	EXPECT_EQ(f, VectorMod16(35464));
}

TEST(VectorMod16, OperatorMinus)
{
	VectorMod16 a(40000);
	VectorMod16 b(21000);

	VectorMod16 c = (a -= b);
	EXPECT_EQ(a, VectorMod16(19000));
	EXPECT_EQ(c, VectorMod16(19000));

	VectorMod16 d = a - b;
	EXPECT_EQ(d, VectorMod16(63536));

	VectorMod16 e = a - 21000;
	EXPECT_EQ(e, VectorMod16(63536));

	VectorMod16 f = 19000 - b;
	EXPECT_EQ(f, VectorMod16(63536));
}

TEST(VectorMod16, OperatorMultiply)
{
	VectorMod16 a(30);
	VectorMod16 b(1000);

	VectorMod16 c = (a *= b);
	EXPECT_EQ(a, VectorMod16(30000));
	EXPECT_EQ(c, VectorMod16(30000));

	VectorMod16 d = a * b;
	EXPECT_EQ(d, VectorMod16(50048));

	VectorMod16 e = a * 1000;
	EXPECT_EQ(e, VectorMod16(50048));

	VectorMod16 f = 30000 * b;
	EXPECT_EQ(f, VectorMod16(50048));
}

TEST(VectorMod16, Conversion)
{
	{
		VectorMod16 in(1000);
		EXPECT_EQ(VectorFracSat16(-31768), in.convert_contiguous());
	}

	{
		VectorMod16 in(1000);
		EXPECT_EQ(VectorFracSat16(1000), static_cast<VectorFracSat16>(in));
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
