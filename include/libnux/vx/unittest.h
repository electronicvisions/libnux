#pragma once

#include <cstdint>

namespace libnux::vx {

uint32_t test_write_string(char const * str);
uint32_t test_write_int(uint32_t const n);

/**
 * Null-terminate the test result.
 */
void test_write_termination();

void test_init(void);
void test_shutdown(void);

typedef enum {
	test_action_warning,
	test_action_shutdown
} test_action_type;

uint32_t test_get_passed();
uint32_t test_get_failed();
uint32_t testcase_get_passed();
uint32_t testcase_get_failed();

void test_set_action(test_action_type const);
test_action_type test_get_action();

void test_inc_passed(void);
void test_inc_failed(void);

/* Print macros */

#define LIBNUX_TEST_STRINGIFY(x) #x
#define LIBNUX_TEST_TO_STRING(x) LIBNUX_TEST_STRINGIFY(x)

#ifdef LIBNUX_TEST_MODE_VERBOSE
#define LIBNUX_TEST_WRITE_PASSED(msg, args)                                                        \
	do {                                                                                           \
		test_write_string(__FILE__);                                                               \
		test_write_string(":");                                                                    \
		test_write_string(LIBNUX_TEST_TO_STRING(__LINE__));                                        \
		test_write_string(": passed: ");                                                           \
		test_write_string(msg);                                                                    \
		test_write_string("(");                                                                    \
		test_write_string(args);                                                                   \
		test_write_string(")\n");                                                                  \
	} while (0)
#else
#define LIBNUX_TEST_WRITE_PASSED(msg, args)
#endif /* write_passed */

#define LIBNUX_TEST_WRITE_FAILED(msg, args)                                                        \
	do {                                                                                           \
		test_write_string(__FILE__);                                                               \
		test_write_string(":");                                                                    \
		test_write_string(LIBNUX_TEST_TO_STRING(__LINE__));                                        \
		test_write_string(": failed: ");                                                           \
		test_write_string(msg);                                                                    \
		test_write_string("(");                                                                    \
		test_write_string(args);                                                                   \
		test_write_string(")\n");                                                                  \
	} while (0)

#define LIBNUX_TEST_FAILED(msg, args)                                                              \
	do {                                                                                           \
		LIBNUX_TEST_WRITE_FAILED(msg, args);                                                       \
		test_inc_failed();                                                                         \
		if (test_get_action() == test_action_shutdown) {                                           \
			test_shutdown();                                                                       \
		}                                                                                          \
	} while (0)

#define LIBNUX_TEST_PASSED(msg, args)                                                              \
	do {                                                                                           \
		LIBNUX_TEST_WRITE_PASSED(msg, args);                                                       \
		test_inc_passed();                                                                         \
	} while (0)

/* Check macros */

#define LIBNUX_TEST(cond, msg, args)                                                               \
	do {                                                                                           \
		if ((cond)) {                                                                              \
			LIBNUX_TEST_PASSED(msg, args);                                                         \
		} else {                                                                                   \
			LIBNUX_TEST_FAILED(msg, args);                                                         \
		}                                                                                          \
	} while (0)

#define LIBNUX_TEST_EQUAL(fst, snd) LIBNUX_TEST((fst) == (snd), "equal", #fst ", " #snd)

#define LIBNUX_TEST_TRUE(cond) LIBNUX_TEST((cond), "true", #cond)

#define LIBNUX_TEST_NULL(ptr) LIBNUX_TEST((ptr) == (void*) 0, "null", #ptr)

#define LIBNUX_TEST_NOT_NULL(ptr) LIBNUX_TEST((ptr) != (void*) 0, "not_null", #ptr)

/* Check functions */

template <typename T, typename U>
void test_equal(const T& fst, const U& snd)
{
	LIBNUX_TEST_EQUAL(fst, snd);
}

template <typename T>
void test_true(const T& cond)
{
	LIBNUX_TEST_TRUE(cond);
}

template <typename T>
void test_null(const T& ptr)
{
	LIBNUX_TEST_NULL(ptr);
}

template <typename T>
void test_not_null(const T& ptr)
{
	LIBNUX_TEST_NOT_NULL(ptr);
}

/* Testcases */

void testcase_begin(char const * name);
void testcase_end(void);

/* Test summary */

void test_summary(void);

} // namespace libnux::vx
