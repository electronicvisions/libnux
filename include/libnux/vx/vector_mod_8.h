#pragma once
#include "libnux/vx/attrib.h"
#include <cstddef>
#include <s2pp.h>

namespace libnux::vx {

struct VectorFracSat8;

/**
 * Vector-unit type with 8-bit unsigned elements and modulo arithmetic.
 */
struct VectorMod8
{
	typedef __vector uint8_t Data;
	Data data;

	typedef uint8_t Element;

	/**
	 * Number of elements in vector.
	 */
	constexpr static size_t size = sizeof(Data);

	/**
	 * Default construct vector.
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8();

	/**
	 * Construct vector by broadcasting given element value.
	 * @param value Element value to broadcast
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8(Element value);

	/**
	 * Construct vector from data.
	 * @param value Data to use
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8(Data const& value);

	/**
	 * Copy constructor.
	 * @param other Instance to copy from
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8(VectorMod8 const& other);

	/**
	 * Copy assignment operator.
	 * @param other Instance to copy from
	 * @return Reference to this instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator=(VectorMod8 const& other);

	/**
	 * Access element of vector.
	 * @param index Index to element to access
	 * @return Vector element
	 */
	ATTRIB_ALWAYS_INLINE Element& operator[](size_t index);

	/**
	 * Access element of vector.
	 * @param index Index to element to access
	 * @return Vector element
	 */
	ATTRIB_ALWAYS_INLINE Element const& operator[](size_t index) const;

	/**
	 * Equality operator.
	 * @param other Vector to compare to
	 * @return Boolean equality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator==(VectorMod8 const& other) const;

	/**
	 * Unequality operator.
	 * @param other Vector to compare to
	 * @return Boolean unequality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator!=(VectorMod8 const& other) const;

	/**
	 * Unaryminus operator.
	 * @return Instance with flipped sign
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator-() const;

	/**
	 * Add rhs to instance.
	 * @param rhs Other value to add to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator+=(VectorMod8 const& rhs);

	/**
	 * Subtract rhs from instance.
	 * @param rhs Other value to subtract from this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator-=(VectorMod8 const& rhs);

	/**
	 * Multiply rhs with instance.
	 * @param rhs Other value to multiply with to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator*=(VectorMod8 const& rhs);

	/**
	 * Shift this instance left element-wise by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator<<=(int num_bits);

	/**
	 * Shift this instance right element-wise by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8& operator>>=(int num_bits);

	/**
	 * Add instance and rhs.
	 * @param rhs Other value to add to this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator+(VectorMod8 const& rhs) const;

	/**
	 * Subtract instance and rhs.
	 * @param rhs Other value to subtract from this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator-(VectorMod8 const& rhs) const;

	/**
	 * Multiply instance and rhs.
	 * @param rhs Other value to multiply with this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator*(VectorMod8 const& rhs) const;

	/**
	 * Element-wise left shift by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator<<(int num_bits) const;

	/**
	 * Element-wise right shift by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod8 operator>>(int num_bits) const;

	/**
	 * Convert unsigned vector with modulo arithmetic to signed vector with fractional arithmetic.
	 * The conversion is contiguous such that 0 -> -128 and 255 -> 127.
	 */
	ATTRIB_ALWAYS_INLINE VectorFracSat8 convert_contiguous() const;

	/**
	 * Convert unsigned vector with modulo arithmetic to signed vector with fractional arithmetic.
	 * The conversion is not contiguous and equivalent to a static_cast of the element types.
	 */
	ATTRIB_ALWAYS_INLINE explicit operator VectorFracSat8() const;
};

/**
 * Add 8-bit unsigned vector with modulo to broadcasted 8-bit unsigned
 * element value.
 * @param lhs VectorMod8::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod8 operator+(VectorMod8::Element lhs, VectorMod8 const& rhs);

/**
 * Subtract 8-bit unsigned vector with modulo arithmetics from broadcasted 8-bit unsigned
 * element value.
 * @param lhs VectorMod8::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod8 operator-(VectorMod8::Element lhs, VectorMod8 const& rhs);

/**
 * Multiply 8-bit unsigned vector with modulo arithmetics with broadcasted 8-bit unsigned
 * element value.
 * @param lhs VectorMod8::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod8 operator*(VectorMod8::Element lhs, VectorMod8 const& rhs);

} // namespace libnux::vx

#include "libnux/vx/vector_mod_8.tcc"
