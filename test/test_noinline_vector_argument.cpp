#include <s2pp.h>
#include "libnux/spr.h"

// not inlined function with vector argument
__attribute__((noinline)) void foo_noinline(vector uint8_t& y)
{
	asm volatile(
		"fxvcmpb %[v]\n"
		: : [v] "kv"(y) :
	);
}

// apparently needed to trigger error
__attribute__((noinline)) time_base_t trigger()
{
	return get_time_base();
}

void test_noinline()
{
	vector uint8_t y = vec_splat_u8(0);
	trigger();
	foo_noinline(y);
}

int32_t start(void)
{
	test_noinline();
	return 0;
}
