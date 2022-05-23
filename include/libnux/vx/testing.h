#pragma once

#include <stddef.h>
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

class TestFactory
{
private:
	typedef void (*TestRunFunc)(void);

	static constexpr size_t max_num_tests = 32;
	static size_t id;
	static TestRunFunc creators[max_num_tests];

public:
	static void register_test(TestRunFunc creator);

	static int call_all();
};

int RUN_ALL_TESTS();

#define TEST(Class, Name)                                                                          \
	class Test_##Class##_##Name                                                                    \
	{                                                                                              \
	private:                                                                                       \
		static void run();                                                                         \
		static void test();                                                                        \
                                                                                                   \
	public:                                                                                        \
		static const char* name() { return "Test(" #Class "." #Name ")"; }                         \
		Test_##Class##_##Name() { TestFactory::register_test(&Test_##Class##_##Name::run); }       \
	} Test_##Class##_##Name##_Instance;                                                            \
                                                                                                   \
	void Test_##Class##_##Name::run()                                                              \
	{                                                                                              \
		testcase_begin(name());                                                             \
		test();                                                                                    \
		testcase_end();                                                                     \
	}                                                                                              \
                                                                                                   \
	void Test_##Class##_##Name::test()

#define EXPECT_EQ(A, B) LIBNUX_TEST((A) == (B), "==", #A ", " #B)
#define EXPECT_NE(A, B) LIBNUX_TEST((A) != (B), "!=", #A ", " #B)
#define EXPECT_GT(A, B) LIBNUX_TEST((A) > (B), ">", #A ", " #B)
#define EXPECT_GE(A, B) LIBNUX_TEST((A) >= (B), ">=", #A ", " #B)
#define EXPECT_LT(A, B) LIBNUX_TEST((A) < (B), "<", #A ", " #B)
#define EXPECT_LE(A, B) LIBNUX_TEST((A) <= (B), "<=", #A ", " #B)

#define EXPECT_TRUE(A) LIBNUX_TEST((A), "true", #A)
#define EXPECT_FALSE(A) LIBNUX_TEST(!(A), "false", #A)

#define ASSERT_EQ(A, B) EXPECT_EQ(A, B)
#define ASSERT_NE(A, B) EXPECT_NE(A, B)
#define ASSERT_GT(A, B) EXPECT_GT(A, B)
#define ASSERT_GE(A, B) EXPECT_GE(A, B)
#define ASSERT_LT(A, B) EXPECT_LT(A, B)
#define ASSERT_LE(A, B) EXPECT_LE(A, B)
#define ASSERT_TRUE(A) EXPECT_TRUE(A)
#define ASSERT_FALSE(A) EXPECT_FALSE(A)
