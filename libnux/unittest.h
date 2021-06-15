#pragma once

#include <stdint.h>

uint32_t libnux_test_write_string(char const * str);
uint32_t libnux_test_write_int(uint32_t const n);

/**
 * Null-terminate the test result.
 */
void libnux_test_write_termination();

void libnux_test_init(void);
void libnux_test_shutdown(void);

typedef enum {
	libnux_test_action_warning,
	libnux_test_action_shutdown
} libnux_test_action_type;

uint32_t libnux_test_get_passed();
uint32_t libnux_test_get_failed();
uint32_t libnux_testcase_get_passed();
uint32_t libnux_testcase_get_failed();

void libnux_test_set_action(libnux_test_action_type const);
libnux_test_action_type libnux_test_get_action();

void libnux_test_inc_passed(void);
void libnux_test_inc_failed(void);

/* Print macros */

#define LIBNUX_TEST_STRINGIFY(x) #x
#define LIBNUX_TEST_TO_STRING(x) LIBNUX_TEST_STRINGIFY(x)

#ifdef LIBNUX_TEST_MODE_VERBOSE
#define LIBNUX_TEST_WRITE_PASSED(msg, args) \
	do { \
		libnux_test_write_string(__FILE__); \
		libnux_test_write_string(":"); \
		libnux_test_write_string(LIBNUX_TEST_TO_STRING(__LINE__)); \
		libnux_test_write_string(": passed: "); \
		libnux_test_write_string(msg); \
		libnux_test_write_string("("); \
		libnux_test_write_string(args); \
		libnux_test_write_string(")\n"); \
	} while (0)
#else
#define LIBNUX_TEST_WRITE_PASSED(msg, args)
#endif /* write_passed */

#define LIBNUX_TEST_WRITE_FAILED(msg, args) \
	do { \
		libnux_test_write_string(__FILE__); \
		libnux_test_write_string(":"); \
		libnux_test_write_string(LIBNUX_TEST_TO_STRING(__LINE__)); \
		libnux_test_write_string(": failed: "); \
		libnux_test_write_string(msg); \
		libnux_test_write_string("("); \
		libnux_test_write_string(args); \
		libnux_test_write_string(")\n"); \
	} while (0)

#define LIBNUX_TEST_FAILED(msg, args) \
	do { \
		LIBNUX_TEST_WRITE_FAILED(msg, args); \
		libnux_test_inc_failed(); \
		if (libnux_test_get_action() == libnux_test_action_shutdown) { \
			libnux_test_shutdown(); \
		} \
	} while (0)

#define LIBNUX_TEST_PASSED(msg, args) \
	do { \
		LIBNUX_TEST_WRITE_PASSED(msg, args); \
		libnux_test_inc_passed(); \
	} while (0)

/* Check macros */

#define LIBNUX_TEST(cond, msg, args) \
	do { \
		if ( (cond) ) { \
			LIBNUX_TEST_PASSED(msg, args); \
		} else { \
			LIBNUX_TEST_FAILED(msg, args); \
		} \
	} while (0)

#define LIBNUX_TEST_EQUAL(fst, snd) \
	LIBNUX_TEST( (fst) == (snd), "equal", #fst ", " #snd)

#define LIBNUX_TEST_TRUE(cond) \
	LIBNUX_TEST( (cond), "true", #cond)

#define LIBNUX_TEST_NULL(ptr) \
	LIBNUX_TEST( (ptr) == (void*)0, "null", #ptr)

#define LIBNUX_TEST_NOT_NULL(ptr) \
	LIBNUX_TEST( (ptr) != (void*)0, "not_null", #ptr)

/* Check functions */

template<typename T, typename U>
void libnux_test_equal(const T &fst, const U &snd)
{
	LIBNUX_TEST_EQUAL(fst, snd);
}

template<typename T>
void libnux_test_true(const T &cond)
{
	LIBNUX_TEST_TRUE(cond);
}

template<typename T>
void libnux_test_null(const T &ptr)
{
	LIBNUX_TEST_NULL(ptr);
}

template<typename T>
void libnux_test_not_null(const T &ptr)
{
	LIBNUX_TEST_NOT_NULL(ptr);
}

/* Testcases */

void libnux_testcase_begin(char const * name);
void libnux_testcase_end(void);

/* Test summary */

void libnux_test_summary(void);

/* not needed outside */

#undef LIBNUX_TEST_STRINGIFY
#undef LIBNUX_TEST_TO_STRING
#undef LIBNUX_TEST_WRITE_PASSED
#undef LIBNUX_TEST_WRITE_FAILED
#undef LIBNUX_TEST_FAILED
#undef LIBNUX_TEST_PASSED
#undef LIBNUX_TEST
#undef LIBNUX_TEST_EQUAL
#undef LIBNUX_TEST_TRUE
#undef LIBNUX_TEST_NULL
#undef LIBNUX_TEST_NOT_NULL
