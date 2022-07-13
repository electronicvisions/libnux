#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod8, Construction)
{
	[[maybe_unused]] VectorRowMod8 from_nothing;

	VectorRowMod8 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorRowMod8::HalfRow even(42);
	VectorRowMod8::HalfRow odd(43);
	VectorRowMod8 from_data(even, odd);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i % 2) ? odd[i / 2] : even[i / 2]);
	}
}

TEST(VectorRowMod8, OperatorUnaryminus)
{
	VectorRowMod8 in(-12);
	EXPECT_EQ(-in, VectorRowMod8(12));
}

TEST(VectorRowMod8, OperatorPlus)
{
	VectorRowMod8 a(10);
	VectorRowMod8 b(140);

	VectorRowMod8 c = (a += b);
	EXPECT_EQ(a, VectorRowMod8(150));
	EXPECT_EQ(c, VectorRowMod8(150));

	VectorRowMod8 d = a + b;
	EXPECT_EQ(d, VectorRowMod8(34));

	VectorRowMod8 e = a + 140;
	EXPECT_EQ(e, VectorRowMod8(34));

	VectorRowMod8 f = 150 + b;
	EXPECT_EQ(f, VectorRowMod8(34));
}

int start()
{
	return RUN_ALL_TESTS();
}
