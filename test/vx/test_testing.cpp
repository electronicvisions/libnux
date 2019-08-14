#include "libnux/vx/testing.h"

TEST(TestCase, ASSERT)
{
	ASSERT_EQ(1, 1);
	ASSERT_NE(1, 2);
	ASSERT_GT(2, 1);
	ASSERT_GE(2, 2);
	ASSERT_LT(1, 2);
	ASSERT_LE(2, 2);
	ASSERT_TRUE(true);
	ASSERT_FALSE(false);
}

TEST(TestCase, EXPECT)
{
	EXPECT_EQ(1, 1);
	EXPECT_NE(1, 2);
	EXPECT_GT(2, 1);
	EXPECT_GE(2, 2);
	EXPECT_LT(1, 2);
	EXPECT_LE(2, 2);
	EXPECT_TRUE(true);
	EXPECT_FALSE(false);
}

int start(void)
{
	return RUN_ALL_TESTS();
}
