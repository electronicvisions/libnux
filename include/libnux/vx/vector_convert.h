#pragma once

#include <s2pp.h>

namespace libnux::vx {

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
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (offset)
		:
	);
	// clang-format on
	return tmp;
}

/**
 * Convert int8_t to uint8_t continuously such that -128 -> 0, 127 -> 255.
 */
inline __vector uint8_t int8_to_uint8(__vector int8_t a)
{
	__vector uint8_t tmp;
	__vector uint8_t offset = vec_splat_u8(128);
	// clang-format off
	asm volatile (
		"fxvaddbm %[tmp], %[a], %[b]\n"
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (offset)
		:
	);
	// clang-format on
	return tmp;
}

/**
 * Convert uint16_t to int16_t continuously such that 0 -> -32768, 65535 -> 32767.
 */
inline __vector int16_t uint16_to_int16(__vector uint16_t a)
{
	__vector int16_t tmp;
	__vector uint16_t offset = vec_splat_u16(32768);
	// clang-format off
	asm volatile (
		"fxvsubhm %[tmp], %[a], %[b]\n"
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (offset)
		:
	);
	// clang-format on
	return tmp;
}

/**
 * Convert int16_t to uint16_t continuously such that -32768 -> 0, 32767 -> 65535.
 */
inline __vector uint16_t int16_to_uint16(__vector int16_t a)
{
	__vector uint16_t tmp;
	__vector uint16_t offset = vec_splat_u16(32768);
	// clang-format off
	asm volatile (
		"fxvaddhm %[tmp], %[a], %[b]\n"
		: [tmp] "=qv" (tmp)
		: [a] "qv" (a),
		  [b] "qv" (offset)
		:
	);
	// clang-format on
	return tmp;
}

} // namespace libnux::vx
