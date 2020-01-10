#include <s2pp.h>
#include "libnux/spr.h"

// inlined function with vector argument
inline __attribute__((always_inline)) void foo_inline(__vector uint8_t& y)
{
	asm volatile(
		"fxvcmpb %[v]\n"
		:
#ifndef LIBNUX_DLS_VERSION_VX
		: [v] "kv" (y)
#else
		: [v] "qv" (y)
#endif
		:
	);
}

// apparently needed to trigger error
__attribute__((noinline)) time_base_t trigger()
{
	return get_time_base();
}

void test_inline()
{
	__vector uint8_t y = vec_splat_u8(0);
	trigger();
	foo_inline(y);
}

uint32_t start(void)
{
	test_inline();
	return 0;
}
