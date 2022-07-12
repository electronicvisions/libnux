#pragma once
#include "libnux/system.h"
#include "libnux/vx/attrib.h"
#include "libnux/vx/detail/vector_traits.h"
#include "libnux/vx/vector_if.h"
#include <cstdint>
#include <s2pp.h>
#include <type_traits>

namespace libnux::vx {

namespace detail {

template <typename VectorMask>
ATTRIB_ALWAYS_INLINE void compare(VectorMask const& mask)
{
	static_assert(
	    sizeof(typename VectorMask::Element) <= 2,
	    "Only byte and halfword elements are supported for comparison.");
	if constexpr (sizeof(typename VectorMask::Element) == 1) {
		asm volatile("fxvcmpb %[mask]\n" : : [mask] "qv"(mask.data) :);
	} else if (sizeof(typename VectorMask::Element) == 2) {
		asm volatile("fxvcmph %[mask]\n" : : [mask] "qv"(mask.data) :);
	}
}

template <typename VectorValue>
ATTRIB_ALWAYS_INLINE VectorValue
select_greater_zero(VectorValue const& value_true, VectorValue const& value_false)
{
	VectorValue ret;
	asm volatile("fxvsel %[ret], %[value_false], %[value_true], %[cond]\n"
	             : [ret] "=qv"(ret.data)
	             : [value_true] "qv"(value_true.data), [value_false] "qv"(value_false.data),
	               [cond] "I"(__C_GT)
	             :);
	return ret;
}

template <typename VectorValue>
ATTRIB_ALWAYS_INLINE VectorValue
select_lesser_zero(VectorValue const& value_true, VectorValue const& value_false)
{
	VectorValue ret;
	asm volatile("fxvsel %[ret], %[value_false], %[value_true], %[cond]\n"
	             : [ret] "=qv"(ret.data)
	             : [value_true] "qv"(value_true.data), [value_false] "qv"(value_false.data),
	               [cond] "I"(__C_LT)
	             :);
	return ret;
}

template <typename VectorValue>
ATTRIB_ALWAYS_INLINE VectorValue
select_equal_zero(VectorValue const& value_true, VectorValue const& value_false)
{
	VectorValue ret;
	asm volatile("fxvsel %[ret], %[value_false], %[value_true], %[cond]\n"
	             : [ret] "=qv"(ret.data)
	             : [value_true] "qv"(value_true.data), [value_false] "qv"(value_false.data),
	               [cond] "I"(__C_EQ)
	             :);
	return ret;
}

} // namespace detail

template <typename VectorMask, typename VectorValue>
VectorValue vector_if(
    VectorMask const& mask,
    VectorIfCondition const condition,
    [[maybe_unused]] VectorValue const& value_true,
    [[maybe_unused]] VectorValue const& value_false)
{
	static_assert(
	    sizeof(typename VectorMask::Element) == sizeof(typename VectorValue::Element),
	    "Element size of mask and value need to match.");
	static_assert(
	    VectorMask::size == VectorValue::size, "Element number of mask and value need to match.");

	if constexpr (detail::IsVector<VectorValue>::value) {
		detail::compare(mask);
		if constexpr (std::is_unsigned_v<typename VectorMask::Element>) {
			switch (condition) {
				case VectorIfCondition::greater:
					return detail::select_equal_zero(value_false, value_true);
				case VectorIfCondition::lesser:
					return value_false;
				case VectorIfCondition::equal:
					return detail::select_equal_zero(value_true, value_false);
				case VectorIfCondition::greater_equal:
					return value_true;
				case VectorIfCondition::lesser_equal:
					return detail::select_equal_zero(value_true, value_false);
				case VectorIfCondition::unequal:
					return detail::select_equal_zero(value_false, value_true);
				default: {
					exit(1);
				}
			}
		} else {
			switch (condition) {
				case VectorIfCondition::greater:
					return detail::select_greater_zero(value_true, value_false);
				case VectorIfCondition::lesser:
					return detail::select_lesser_zero(value_true, value_false);
				case VectorIfCondition::equal:
					return detail::select_equal_zero(value_true, value_false);
				case VectorIfCondition::greater_equal:
					return detail::select_lesser_zero(value_false, value_true);
				case VectorIfCondition::lesser_equal:
					return detail::select_greater_zero(value_false, value_true);
				case VectorIfCondition::unequal:
					return detail::select_equal_zero(value_false, value_true);
				default: {
					exit(1);
				}
			}
		}
	} else if constexpr (detail::IsVectorHalfRow<VectorValue>::value) {
		return VectorValue(
		    vector_if(mask.left, condition, value_true.left, value_false.left),
		    vector_if(mask.right, condition, value_true.right, value_false.right));
	} else if constexpr (detail::IsVectorRow<VectorValue>::value) {
		return VectorValue(
		    vector_if(mask.even, condition, value_true.even, value_false.even),
		    vector_if(mask.odd, condition, value_true.odd, value_false.odd));
	}
	exit(1);
	// never reached
	return {};
}

} // namespace libnux::vx
