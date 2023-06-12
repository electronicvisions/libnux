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

} // namespace libnux::vx
