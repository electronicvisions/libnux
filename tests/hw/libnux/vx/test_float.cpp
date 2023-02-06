#include "libnux/vx/attrib.h"
#include "libnux/vx/helper.h"
#include "libnux/vx/time.h"
#include "libnux/vx/random.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/testing.h"

using namespace libnux::vx;

uint32_t seed = 1234;
constexpr size_t num  = 10;

float random_float()
{
	auto const tmp = xorshift32(&seed);
	return *reinterpret_cast<float const*>(&tmp);
}

void test_in_range(uint32_t value, uint32_t mean, uint32_t maxerr)
{
	test_true((value <= mean + maxerr) && (value >= mean - maxerr));
}

#define FLOAT_MEASURE(Name, OP) \
time_base_t measure_##Name() \
{\
\
	float a[num];\
	float b[num];\
	for (size_t i = 0; i < num; ++i) {\
		a[i] = random_float();\
		b[i] = random_float();\
	}\
	ATTRIB_UNUSED float c = {};\
\
	time_base_t const begin = now();\
\
	for (size_t i = 0; i < num; ++i) {\
		do_not_optimize_away(a[i]);\
		do_not_optimize_away(b[i]);\
		do_not_optimize_away(c);\
		c = a[i] OP b[i];\
	}\
\
	time_base_t const end = now();\
\
	return (end - begin) / num;\
}

FLOAT_MEASURE(multiplication, *)
FLOAT_MEASURE(addition, +)
FLOAT_MEASURE(subtraction, -)
FLOAT_MEASURE(division, /)

#define FLOAT_TEST(Name, Mean, StdDev) \
	testcase_begin(#Name); \
	auto const duration_##Name = measure_##Name();\
	mailbox_write_int(duration_##Name);\
	mailbox_write_string("\n");\
	test_in_range(duration_##Name, Mean, StdDev);\
	testcase_end();

TEST(Performance, General) {
	FLOAT_TEST(multiplication, 1000, 1000)
	FLOAT_TEST(addition, 1000, 1000)
	FLOAT_TEST(subtraction, 1000, 1000)
	FLOAT_TEST(division, 1000, 1000)
}


int start()
{
	return RUN_ALL_TESTS();
}
