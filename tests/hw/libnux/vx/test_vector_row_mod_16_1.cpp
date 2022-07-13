#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowMod16, Construction)
{
	[[maybe_unused]] VectorRowMod16 from_nothing;

	VectorRowMod16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorRowMod16::HalfRow even(42);
	VectorRowMod16::HalfRow odd(43);
	VectorRowMod16 from_data(even, odd);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i % 2) ? odd[i / 2] : even[i / 2]);
	}
}

TEST(VectorRowMod16, OperatorUnaryminus)
{
	VectorRowMod16 in(-12);
	EXPECT_EQ(-in, VectorRowMod16(12));
}

TEST(VectorRowMod16, OperatorPlus)
{
	VectorRowMod16 a(1000);
	VectorRowMod16 b(50000);

	VectorRowMod16 c = (a += b);
	EXPECT_EQ(a, VectorRowMod16(51000));
	EXPECT_EQ(c, VectorRowMod16(51000));

	VectorRowMod16 d = a + b;
	EXPECT_EQ(d, VectorRowMod16(35464));

	VectorRowMod16 e = a + 50000;
	EXPECT_EQ(e, VectorRowMod16(35464));

	VectorRowMod16 f = 51000 + b;
	EXPECT_EQ(f, VectorRowMod16(35464));
}

int start()
{
	return RUN_ALL_TESTS();
}
