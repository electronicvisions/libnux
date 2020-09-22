#pragma once
#include <s2pp.h>

namespace libnux {

/**
 * Convert uint8_t to int8_t continuously such that 0 -> -128, 255 -> 127.
 */
inline __vector int8_t uint8_to_int8(__vector uint8_t a)
{
	__vector int8_t tmp;
	__vector uint8_t offset = vec_splat_u8(128);
	// clang-format off
	asm volatile (
		"fxvsubbm %[tmp], %[a], %[b]\n"
#ifdef LIBNUX_DLS_VERSION_VX
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (offset)
#else
		: [tmp] "=kv" (tmp)
		: [a] "kv" (a),
		  [b] "kv" (offset)
#endif
		:
	);
	// clang-format on
	return tmp;
}

} // namespace libnux
