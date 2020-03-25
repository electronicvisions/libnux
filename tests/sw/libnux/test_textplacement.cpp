#include "libnux/vx/attrib.h"

// explicitly place code to internal memory
int start() __attribute__((section("int.text")));
int test_explicit_int() ATTRIB_LINK_TO_INTERNAL;
int test_explicit_ext() ATTRIB_LINK_TO_EXTERNAL __attribute__((visibility("hidden")));
int test_implicit_ext();

#pragma GCC push_options
#pragma GCC optimize("O0")

int start()
{
	auto a = test_explicit_int();
	auto b = test_explicit_ext();
	auto c = test_implicit_ext();

	if ((a + b + c) != 6) {
		return 1;
	}

	return 0;
}
#pragma GCC pop_options

int test_explicit_int()
{
	return 1;
}

int test_explicit_ext()
{
	return 2;
}

int test_implicit_ext()
{
	return 3;
}
