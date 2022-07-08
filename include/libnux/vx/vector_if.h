#pragma once
#include "libnux/vx/attrib.h"

namespace libnux::vx {

/**
 * Condition of vector vector_if operation.
 */
enum class VectorIfCondition
{
	greater,
	lesser,
	equal,
	greater_equal,
	lesser_equal,
	unequal,
};


/**
 * Select between values of two vectors via applying condition of a mask and zero: mask condition 0.
 * Equivalent to element-wise operation:
 *     if (mask[i] condition 0) {
 *         return value_true[i];
 *     } else {
 *         return value_false[i];
 *     }
 * Unsigned values can never be smaller than zero.
 *
 * @param mask Mask to apply with condition
 * @param condition Condition to apply
 * @param value_true Value to select when condition is fulfilled
 * @param value_false Value to select when condition is not fulfilled
 */
template <typename VectorMask, typename VectorValue>
ATTRIB_ALWAYS_INLINE VectorValue vector_if(
    VectorMask const& mask,
    VectorIfCondition const condition,
    VectorValue const& value_true,
    VectorValue const& value_false);

} // namespace libnux::vx

#include "libnux/vx/vector_if.tcc"
