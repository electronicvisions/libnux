#pragma once
#include "libnux/vx/attrib.h"
#include "libnux/vx/vector_half_row_frac_sat_16.h"
#include "libnux/vx/vector_half_row_frac_sat_8.h"
#include "libnux/vx/vector_half_row_mod_16.h"
#include "libnux/vx/vector_half_row_mod_8.h"
#include <cstddef>
#include <utility>

namespace libnux::vx {

/**
 * Vector row (of synapse matrix).
 */
template <typename VectorHalfRow>
struct VectorRow
{
	typedef VectorHalfRow HalfRow;
	// parallel access to observables on hardware is interleaved
	HalfRow even{};
	HalfRow odd{};

	typedef typename HalfRow::Element Element;

	/**
	 * Number of elements in vector row.
	 */
	constexpr static size_t size = HalfRow::size * 2;

	/**
	 * Default construct vector row.
	 */
	ATTRIB_ALWAYS_INLINE VectorRow() = default;

	/**
	 * Construct vector by filling with given scalar value.
	 * @param value Element value to fill into vector
	 */
	ATTRIB_ALWAYS_INLINE VectorRow(Element value);

	/**
	 * Construct vector from interleaving half rows.
	 * @param even Data to use in even-indexed vector elements
	 * @param odd Data to use in odd-indexed vector elements
	 */
	ATTRIB_ALWAYS_INLINE VectorRow(HalfRow const& even, HalfRow const& odd);

	/**
	 * Copy constructor.
	 * @param other Instance to copy from
	 */
	ATTRIB_ALWAYS_INLINE VectorRow(VectorRow const& other);

	/**
	 * Copy assignment operator.
	 * @param other Instance to copy from
	 * @return Reference to this instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator=(VectorRow const& other);

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
	ATTRIB_ALWAYS_INLINE bool operator==(VectorRow const& other) const;

	/**
	 * Unequality operator.
	 * @param other Vector to compare to
	 * @return Boolean unequality value
	 */
	ATTRIB_ALWAYS_INLINE bool operator!=(VectorRow const& other) const;

	/**
	 * Unaryminus operator.
	 * @return Instance with flipped sign
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator-() const;

	/**
	 * Add rhs to instance.
	 * @param rhs Other value to add to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator+=(VectorRow const& rhs);

	/**
	 * Subtract rhs from instance.
	 * @param rhs Other value to subtract from this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator-=(VectorRow const& rhs);

	/**
	 * Multiply rhs with instance.
	 * @param rhs Other value to multiply with to this instance
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator*=(VectorRow const& rhs);

	/**
	 * Shift this instance left element-wise by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator<<=(int num_bits);

	/**
	 * Shift this instance right element-wise by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow& operator>>=(int num_bits);

	/**
	 * Add instance and rhs.
	 * @param rhs Other value to add to this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator+(VectorRow const& rhs) const;

	/**
	 * Subtract instance and rhs.
	 * @param rhs Other value to subtract from this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator-(VectorRow const& rhs) const;

	/**
	 * Multiply instance and rhs.
	 * @param rhs Other value to multiply with this instance
	 * @return Result instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator*(VectorRow const& rhs) const;

	/**
	 * Element-wise left shift by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a right shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator<<(int num_bits) const;

	/**
	 * Element-wise right shift by given number of bits.
	 * The shift performed is an arithmetic shift, no saturation is performed.
	 * @param num_bits Number of bits to shift this instance. Negative values yield a left shift.
	 * @return Reference to altered instance
	 */
	ATTRIB_ALWAYS_INLINE VectorRow operator>>(int num_bits) const;

	/**
	 * Contiguously convert vector row to vector row with other element type.
	 */
	ATTRIB_ALWAYS_INLINE VectorRow<decltype(std::declval<HalfRow>().convert_contiguous())>
	convert_contiguous() const;

	/**
	 * Convert vector row to vector row with other element type.
	 * Supported conversions are specialized below.
	 */
	template <typename OtherVectorRow>
	explicit operator OtherVectorRow() const;
};

/**
 * Add vector row to broadcasted element value.
 * @param lhs Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
template <typename HalfRow>
ATTRIB_ALWAYS_INLINE VectorRow<HalfRow> operator+(
    typename VectorRow<HalfRow>::Element lhs, VectorRow<HalfRow> const& rhs);

/**
 * Subtract vector row from broadcasted element value.
 * @param lhs VectorRow::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
template <typename HalfRow>
ATTRIB_ALWAYS_INLINE VectorRow<HalfRow> operator-(
    typename VectorRow<HalfRow>::Element lhs, VectorRow<HalfRow> const& rhs);

/**
 * Multiply vector row with broadcasted element value.
 * @param lhs VectorRow::Element value
 * @param rhs Vector value
 * @return Resulting vector
 */
template <typename HalfRow>
ATTRIB_ALWAYS_INLINE VectorRow<HalfRow> operator*(
    typename VectorRow<HalfRow>::Element lhs, VectorRow<HalfRow> const& rhs);


/**
 * Vector row (of synapse matrix) with 8 bit signed fractional saturating arithmetics.
 */
typedef VectorRow<VectorHalfRowFracSat8> VectorRowFracSat8;

/**
 * Vector row (of synapse matrix) with 16 bit signed fractional saturating arithmetics.
 */
typedef VectorRow<VectorHalfRowFracSat16> VectorRowFracSat16;

/**
 * Vector row (of synapse matrix) with 8 bit unsigned modulo arithmetics.
 */
typedef VectorRow<VectorHalfRowMod8> VectorRowMod8;

/**
 * Vector row (of synapse matrix) with 16 bit unsigned modulo arithmetics.
 */
typedef VectorRow<VectorHalfRowMod16> VectorRowMod16;


/**
 * Convert signed vector row with fractional arithmetic to unsigned vector row with modulo
 * arithmetic. The conversion is contiguous such that -128 -> 0 and 127 -> 255.
 * @return Converted Vector row
 */
template <> template <> ATTRIB_ALWAYS_INLINE VectorRowFracSat8::operator VectorRowMod8() const;

/**
 * Convert signed vector row with fractional arithmetic to unsigned vector row with modulo
 * arithmetic. The conversion is contiguous such that -32768 -> 0 and 32767 -> 65535.
 * @return Converted Vector row
 */
template <> template <> ATTRIB_ALWAYS_INLINE VectorRowFracSat16::operator VectorRowMod16() const;

/**
 * Convert unsigned vector row with modulo arithmetic to signed vector row with fractional
 * arithmetic. The conversion is contiguous such that 0 -> -128 and 255 -> 127.
 * @return Converted Vector row
 */
template <> template <> ATTRIB_ALWAYS_INLINE VectorRowMod8::operator VectorRowFracSat8() const;

/**
 * Convert unsigned vector row with modulo arithmetic to signed vector row with fractional
 * arithmetic. The conversion is contiguous such that 0-> -32768 and 65535 -> 32767.
 * @return Converted Vector row
 */
template <> template <> ATTRIB_ALWAYS_INLINE VectorRowMod16::operator VectorRowFracSat16() const;

/**
 * Convert unsigned vector row with modulo arithmetic from 8-bit to 16-bit.
 * The conversion sets the lower 8 bits of the 16-bit values and therefore is an identity because
 * all shifts can be achieved by a followed multiplication (or shift).
 * @param value Vector row to convert
 * @return Converted Vector row
 */
template <>
template <>
ATTRIB_ALWAYS_INLINE VectorRowMod8::operator VectorRowMod16() const;

/**
 * Convert unsigned vector row with modulo arithmetic from 16-bit to 8-bit.
 * The conversion uses the lower 8 bits of the 16-bit values ignores the upper 8 bits and therefore
 * is identical to a static_cast. All shifts can be achieved by previously.
 * @param value Vector row to convert
 * @return Converted Vector row
 */
template <>
template <>
ATTRIB_ALWAYS_INLINE VectorRowMod16::operator VectorRowMod8() const;

/**
 * Convert signed vector row with fractional saturating arithmetic from 8-bit to 16-bit.
 * The conversion sets the higher 8 bits of the 16-bit values and therefore preserves the sign and
 * is an identity when identifying the values with the interval [0,1), but is multiplied by 256 when
 * comparing the raw integer values. Like this all shifts or divisions can be achieved by a followed
 * multiplication.
 * @param value Vector row to convert
 * @return Converted Vector row
 */
template <>
template <>
ATTRIB_ALWAYS_INLINE VectorRowFracSat8::operator VectorRowFracSat16() const;

/**
 * Convert signed vector row with fractional saturating arithmetic from 16-bit to 8-bit.
 * The conversion uses the higher 8 bits of the 16-bit values and therefore preserves the sign and
 * is an identity when identifying the values with the interval [0,1), but is divided by 256 when
 * comparing the raw integer values. The lower 8 bits are ignored.
 * @param value Vector row to convert
 * @return Converted Vector row
 */
template <>
template <>
ATTRIB_ALWAYS_INLINE VectorRowFracSat16::operator VectorRowFracSat8() const;


} // namespace libnux::vx

#include "libnux/vx/vector_row.tcc"
