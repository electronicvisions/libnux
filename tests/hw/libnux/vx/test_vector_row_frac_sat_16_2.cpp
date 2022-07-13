#include "libnux/vx/testing.h"
#include "libnux/vx/vector_row.h"


TEST(VectorRowFracSat16, OperatorMinus)
{
	VectorRowFracSat16 a(-3000);
	VectorRowFracSat16 b(20000);

	VectorRowFracSat16 c = (a -= b);
	EXPECT_EQ(a, VectorRowFracSat16(-23000));
	EXPECT_EQ(c, VectorRowFracSat16(-23000));

	VectorRowFracSat16 d = a - b;
	EXPECT_EQ(d, VectorRowFracSat16(-32768));

	VectorRowFracSat16 e = a - 20000;
	EXPECT_EQ(e, VectorRowFracSat16(-32768));

	VectorRowFracSat16 f = -23000 - b;
	EXPECT_EQ(f, VectorRowFracSat16(-32768));
}

int start()
{
	return RUN_ALL_TESTS();
}
