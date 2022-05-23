#pragma once

#include <s2pp.h>

namespace libnux::vx {

inline __vector int8_t saturating_subtract(__vector int8_t a, __vector int8_t b)
{
	__vector int8_t tmp;
	// clang-format off
	asm volatile (
		"fxvsubbfs %[tmp], %[a], %[b]\n"
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (b)
		:
	);
	// clang-format on
	return tmp;
}

} // namespace libnux::vx
