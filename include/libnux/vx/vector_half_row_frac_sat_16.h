#pragma once
#include "libnux/vx/attrib.h"
#include "libnux/vx/vector_frac_sat_16.h"

namespace libnux::vx {

struct VectorHalfRowMod16;

/**
 * Half (of synapse) row with 16-bit signed elements and fractional saturating arithmetics.
 */
struct VectorHalfRowFracSat16
{
	typedef VectorFracSat16 Quarter;
	Quarter left;
	Quarter right;

	typedef Quarter::Element Element;

	/**
	 * Number of elements in vector.
	 */
	constexpr static size_t size = Quarter::size * 2;

	/**
	 * Default construct vector.
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16();

	/**
	 * Construct vector by broadcasting given element value.
	 * @param value Element value to broadcast
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16(Element value);

	/**
	 * Construct vector from data.
	 * @param value Data to use
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16(Quarter const& left, Quarter const& right);

	/**
	 * Copy constructor.
	 * @param other Instance to copy from
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16(VectorHalfRowFracSat16 const& other);

	/**
	 * Copy assignment operator.
	 * @param other Instance to copy from
	 * @return Reference to this instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator=(VectorHalfRowFracSat16 const& other);

	/**
	 * Access element of vector.
	 * @param index Index to element to access
	 * @return VectorHalfRow element
	 */
	ATTRIB_ALWAYS_INLINE Element& operator[](size_t index);

	/**
	 * Access element of vector.
	 * @param index Index to element to access
	 * @return VectorHalfRow element
	 */
	ATTRIB_ALWAYS_INLINE Element const& operator[](size_t index) const;

	/**
	 * Equality operator.
	 * @param other VectorHalfRow to compare to
	 * @return Boolean equality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator==(VectorHalfRowFracSat16 const& other) const;

	/**
	 * Unequality operator.
	 * @param other VectorHalfRow to compare to
	 * @return Boolean unequality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator!=(VectorHalfRowFracSat16 const& other) const;

	/**
	 * Unaryminus operator.
	 * @return Instance with flipped sign
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator-() const;

	/**
	 * Add rhs to instance.
	 * @param rhs Other value to add to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator+=(VectorHalfRowFracSat16 const& rhs);

	/**
	 * Subtract rhs from instance.
	 * @param rhs Other value to subtract from this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator-=(VectorHalfRowFracSat16 const& rhs);

	/**
	 * Multiply rhs with instance.
	 * @param rhs Other value to multiply with to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator*=(VectorHalfRowFracSat16 const& rhs);

	/**
	 * Shift this instance left element-wise by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator<<=(int num_bits);

	/**
	 * Shift this instance right element-wise by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16& operator>>=(int num_bits);

	/**
	 * Add instance and rhs.
	 * @param rhs Other value to add to this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator+(VectorHalfRowFracSat16 const& rhs) const;

	/**
	 * Subtract instance and rhs.
	 * @param rhs Other value to subtract from this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator-(VectorHalfRowFracSat16 const& rhs) const;

	/**
	 * Multiply instance and rhs.
	 * @param rhs Other value to multiply with this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator*(VectorHalfRowFracSat16 const& rhs) const;

	/**
	 * Element-wise left shift by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator<<(int num_bits) const;

	/**
	 * Element-wise right shift by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16 operator>>(int num_bits) const;

	/**
	 * Convert signed vector with fractional arithmetic to unsigned vector with modulo arithmetic.
	 * The conversion is contiguous such that -32768 -> 0 and 32767 -> 65535.
	 */
	ATTRIB_ALWAYS_INLINE VectorHalfRowMod16 convert_contiguous() const;

	/**
	 * Convert signed vector with fractional arithmetic to unsigned vector with modulo arithmetic.
	 * The conversion is not contiguous and equivalent to a static_cast of the element types.
	 */
	ATTRIB_ALWAYS_INLINE explicit operator VectorHalfRowMod16() const;
};

/**
 * Add 16-bit signed vector with fractional saturating arithmetics to broadcasted 16-bit signed
 * element value.
 * @param lhs VectorHalfRowFracSat16::Element value
 * @param rhs VectorHalfRow value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16
operator+(VectorHalfRowFracSat16::Element lhs, VectorHalfRowFracSat16 const& rhs);

/**
 * Subtract 16-bit signed vector with fractional saturating arithmetics from broadcasted 16-bit
 * signed element value.
 * @param lhs VectorHalfRowFracSat16::Element value
 * @param rhs VectorHalfRow value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16
operator-(VectorHalfRowFracSat16::Element lhs, VectorHalfRowFracSat16 const& rhs);

/**
 * Multiply 16-bit signed vector with fractional saturating arithmetics with broadcasted 16-bit
 * signed element value.
 * @param lhs VectorHalfRowFracSat16::Element value
 * @param rhs VectorHalfRow value
 * @return Resulting vector
 */
ATTRIB_ALWAYS_INLINE VectorHalfRowFracSat16
operator*(VectorHalfRowFracSat16::Element lhs, VectorHalfRowFracSat16 const& rhs);

} // namespace libnux::vx

#include "libnux/vx/vector_half_row_frac_sat_16.tcc"
