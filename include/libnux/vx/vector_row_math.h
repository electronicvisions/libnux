#pragma once
#include "libnux/vx/vector_row.h"

namespace libnux::vx {

/**
 * Calculates VectorRow-valued value raised to the power of given exponent.
 * @param base Base value
 * @param exponent Exponent value
 * @return base ^ exponent
 */
template <typename T>
inline VectorRow<T> pow(VectorRow<T> const& base, size_t exponent)
{
	if (exponent == 0) {
		VectorRow<T> ret;
		ret = 1;
		return ret;
	} else if (exponent == 1) {
		return base;
	} else {
		VectorRow<T> ret = base;
		for (size_t i = 0; i < exponent - 1; ++i) {
			ret *= base;
		}
		return ret;
	}
}


/**
 * @brief Saturate vector row to stay in [0,64).
 * @param input Weight vector
 * @return Saturated weight vector
 */
template <typename T>
inline VectorRow<T> saturate_weight(VectorRow<T> const& input)
{
	static_assert(
	    sizeof(typename VectorRow<T>::Element) == 1, "saturate_weight only works with chars.");
	VectorRow<T> ret;
	__vector uint8_t tmp;
	asm volatile(
	    // set <=0 to 0
	    "fxvcmpb %[input_even]\n"
	    "fxvsel %[ret_even], %[zero], %[input_even], %[cond_gt]\n"
	    // set >63 to 63
	    "fxvsubbm %[tmp], %[ret_even], %[const_63]\n"
	    "fxvcmpb %[tmp]\n"
	    "fxvsel %[ret_even], %[ret_even], %[const_63], %[cond_gt]\n"
	    // set <0 to 0
	    "fxvcmpb %[input_odd]\n"
	    "fxvsel %[ret_odd], %[zero], %[input_odd], %[cond_gt]\n"
	    // set >63 to 63
	    "fxvsubbm %[tmp], %[ret_odd], %[const_63]\n"
	    "fxvcmpb %[tmp]\n"
	    "fxvsel %[ret_odd], %[ret_odd], %[const_63], %[cond_gt]\n"
	    : [tmp] "=&qv"(tmp), [ret_even] "=&qv"(*reinterpret_cast<__vector uint8_t*>(&ret.even)),
	      [ret_odd] "=&qv"(*reinterpret_cast<__vector uint8_t*>(&ret.odd))
	    : [input_even] "qv"(*reinterpret_cast<__vector uint8_t const*>(&input.even)),
	      [input_odd] "qv"(*reinterpret_cast<__vector uint8_t const*>(&input.odd)),
	      [zero] "qv"(vec_splat_u8(0)), [const_63] "qv"(vec_splat_u8(63)), [cond_gt] "I"(__C_GT)
	    :);
	return ret;
}

} // namespace libnux::vx
