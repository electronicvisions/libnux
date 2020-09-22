#pragma once
#include <s2pp.h>

namespace libnux {

inline __vector int8_t saturating_subtract(__vector int8_t a, __vector int8_t b)
{
	__vector int8_t tmp;
	// clang-format off
	asm volatile (
		"fxvsubbfs %[tmp], %[a], %[b]\n"
#ifdef LIBNUX_DLS_VERSION_VX
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (b)
#else
		: [tmp] "=kv" (tmp)
		: [a] "kv" (a),
		  [b] "kv" (b)
#endif
		:
	);
	// clang-format on
	return tmp;
}

} // namespace libnux
