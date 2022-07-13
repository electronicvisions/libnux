#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat16, Construction)
{
	[[maybe_unused]] VectorRowFracSat16 from_nothing;

	VectorRowFracSat16 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorRowFracSat16::HalfRow even(42);
	VectorRowFracSat16::HalfRow odd(43);
	VectorRowFracSat16 from_data(even, odd);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i % 2) ? odd[i / 2] : even[i / 2]);
	}
}

TEST(VectorRowFracSat16, OperatorUnaryminus)
{
	VectorRowFracSat16 in(-12);
	EXPECT_EQ(-in, VectorRowFracSat16(12));
}

TEST(VectorRowFracSat16, OperatorPlus)
{
	VectorRowFracSat16 a(-3000);
	VectorRowFracSat16 b(20000);

	VectorRowFracSat16 c = (a += b);
	EXPECT_EQ(a, VectorRowFracSat16(17000));
	EXPECT_EQ(c, VectorRowFracSat16(17000));

	VectorRowFracSat16 d = a + b;
	EXPECT_EQ(d, VectorRowFracSat16(32767));

	VectorRowFracSat16 e = a + 20000;
	EXPECT_EQ(e, VectorRowFracSat16(32767));

	VectorRowFracSat16 f = 17000 + b;
	EXPECT_EQ(f, VectorRowFracSat16(32767));
}

int start()
{
	return RUN_ALL_TESTS();
}
