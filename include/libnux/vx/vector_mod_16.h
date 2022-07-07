#pragma once
#include "libnux/vx/attrib.h"
#include <cstddef>
#include <s2pp.h>

namespace libnux::vx {

struct VectorFracSat16;

/**
 * Vector-unit type with 16-bit unsigned elements and modulo arithmetic.
 */
struct VectorMod16
{
	typedef __vector uint16_t Data;
	Data data;

	typedef uint16_t Element;

	/**
	 * Number of elements in vector.
	 */
	constexpr static size_t size = sizeof(Data) / sizeof(Element);

	/**
	 * Default construct vector.
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16();

	/**
	 * Construct vector by broadcasting given element value.
	 * @param value Element value to broadcast
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16(Element value);

	/**
	 * Construct vector from data.
	 * @param value Data to use
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16(Data const& value);

	/**
	 * Copy constructor.
	 * @param other Instance to copy from
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16(VectorMod16 const& other);

	/**
	 * Copy assignment operator.
	 * @param other Instance to copy from
	 * @return Reference to this instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator=(VectorMod16 const& other);

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
	ATTRIB_ALWAYS_INLINE bool operator==(VectorMod16 const& other) const;

	/**
	 * Unequality operator.
	 * @param other Vector to compare to
	 * @return Boolean unequality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator!=(VectorMod16 const& other) const;

	/**
	 * Unaryminus operator.
	 * @return Instance with flipped sign
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator-() const;

	/**
	 * Add rhs to instance.
	 * @param rhs Other value to add to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator+=(VectorMod16 const& rhs);

	/**
	 * Subtract rhs from instance.
	 * @param rhs Other value to subtract from this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator-=(VectorMod16 const& rhs);

	/**
	 * Multiply rhs with instance.
	 * @param rhs Other value to multiply with to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator*=(VectorMod16 const& rhs);

	/**
	 * Shift this instance left element-wise by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator<<=(int num_bits);

	/**
	 * Shift this instance right element-wise by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16& operator>>=(int num_bits);

	/**
	 * Add instance and rhs.
	 * @param rhs Other value to add to this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator+(VectorMod16 const& rhs) const;

	/**
	 * Subtract instance and rhs.
	 * @param rhs Other value to subtract from this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator-(VectorMod16 const& rhs) const;

	/**
	 * Multiply instance and rhs.
	 * @param rhs Other value to multiply with this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator*(VectorMod16 const& rhs) const;

	/**
	 * Element-wise left shift by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator<<(int num_bits) const;

	/**
	 * Element-wise right shift by given number of bits.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorMod16 operator>>(int num_bits) const;

	/**
	 * Convert unsigned vector with modulo arithmetic to signed vector with fractional arithmetic.
	 * The conversion is contiguous such that 0 -> -32768 and 65535 -> 32767.
	 */
	ATTRIB_ALWAYS_INLINE VectorFracSat16 convert_contiguous() const;

	/**
	 * Convert unsigned vector with modulo arithmetic to signed vector with fractional arithmetic.
	 * The conversion is not contiguous and equivalent to a static_cast of the element types.
	 */
	ATTRIB_ALWAYS_INLINE explicit operator VectorFracSat16() const;
};

/**
 * Add 16-bit unsigned vector with modulo to broadcasted 16-bit unsigned
 * element value.
 * @param lhs VectorMod16::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod16 operator+(VectorMod16::Element lhs, VectorMod16 const& rhs);

/**
 * Subtract 16-bit unsigned vector with modulo arithmetics from broadcasted 16-bit unsigned
 * element value.
 * @param lhs VectorMod16::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod16 operator-(VectorMod16::Element lhs, VectorMod16 const& rhs);

/**
 * Multiply 16-bit unsigned vector with modulo arithmetics with broadcasted 16-bit unsigned
 * element value.
 * @param lhs VectorMod16::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorMod16 operator*(VectorMod16::Element lhs, VectorMod16 const& rhs);

} // namespace libnux::vx

#include "libnux/vx/vector_mod_16.tcc"
