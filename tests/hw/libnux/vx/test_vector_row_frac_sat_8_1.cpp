#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat8, Construction)
{
	[[maybe_unused]] VectorRowFracSat8 from_nothing;

	VectorRowFracSat8 from_int(12);
	for (size_t i = 0; i < from_int.size; ++i) {
		EXPECT_EQ(from_int[i], 12);
	}

	VectorRowFracSat8::HalfRow even(42);
	VectorRowFracSat8::HalfRow odd(43);
	VectorRowFracSat8 from_data(even, odd);
	for (size_t i = 0; i < from_data.size; ++i) {
		EXPECT_EQ(from_data[i], (i % 2) ? odd[i / 2] : even[i / 2]);
	}
}

TEST(VectorRowFracSat8, OperatorUnaryminus)
{
	VectorRowFracSat8 in(-12);
	EXPECT_EQ(-in, VectorRowFracSat8(12));
}

int start()
{
	return RUN_ALL_TESTS();
}
