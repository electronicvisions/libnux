#pragma once
#ifndef __ppu__
#include <iostream>
#else
#include "libnux/vx/mailbox.h"
#endif
#include "libnux/vx/genpybind.h"
#include <cstdint>

namespace libnux::vx GENPYBIND_TAG_LIBNUX_VX {
namespace detail {

struct frac8_t_attributes
{
	typedef int8_t data_type;
	typedef int16_t double_precision_type;

	static constexpr int left = 1;
	static constexpr int right = 8;
	static constexpr int min = INT8_MIN;
	static constexpr int max = INT8_MAX;
};

struct frac16_t_attributes
{
	typedef int16_t data_type;
	typedef int32_t double_precision_type;

	static constexpr int left = 1;
	static constexpr int right = 16;
	static constexpr int min = INT16_MIN;
	static constexpr int max = INT16_MAX;
};

} // namespace detail

/**
 * Saturating fractional data type.
 * @tparam Attr Attributes of fractional data type, i.e range, types, scale.
 */
template <class Attr>
struct GENPYBIND(visible) frac
{
	/// Type used for all calculations for overflow prevention.
	typedef typename Attr::double_precision_type double_precision_type;

	/// Underlying data type.
	typedef typename Attr::data_type data_type;

	/**
	 * Get scalefactor of fractional number in the representation value/scale.
	 * @return Scalefactor
	 */
	constexpr static double_precision_type scale()
	{
		return pow_2(Attr::right - Attr::left, 1);
	}

	/**
	 * Default constructor.
	 * Initialization to 0.
	 */
	constexpr frac() : m_data(0) {}

	/**
	 * Construction from data_type value.
	 */
	constexpr explicit frac(data_type const& data) : m_data(data) {}

	/**
	 * Set by float value.
	 * @param f Float value to convert to fractional representation
	 */
	void from_float(float f) { m_data = static_cast<data_type>(f * static_cast<float>(scale())); }

	/**
	 * Convert to float value.
	 * @return Float value to converted from fractional representation
	 */
	constexpr float to_float() const
	{
		return static_cast<float>(m_data) / static_cast<float>(scale());
	}

	/**
	 * Self-addition operator.
	 * Saturation at minimal and maximal value.
	 * @param other Object to add to instance
	 * @return Reference to altered instance
	 */
	frac& operator+=(frac const& other)
	{
		double_precision_type tmp;
		tmp = static_cast<double_precision_type>(m_data) +
		      static_cast<double_precision_type>(other.m_data);
		tmp = (tmp > Attr::max) ? Attr::max : tmp;
		tmp = (tmp < Attr::min) ? Attr::min : tmp;
		m_data = static_cast<data_type>(tmp);
		return *this;
	}

	/**
	 * Self-subtraction operator.
	 * Saturation at minimal and maximal value.
	 * @param other Object to subtract from instance
	 * @return Reference to altered instance
	 */
	frac& operator-=(frac const& other)
	{
		double_precision_type tmp;
		tmp = static_cast<double_precision_type>(m_data) -
		      static_cast<double_precision_type>(other.m_data);
		tmp = (tmp > Attr::max) ? Attr::max : tmp;
		tmp = (tmp < Attr::min) ? Attr::min : tmp;
		m_data = static_cast<data_type>(tmp);
		return *this;
	}

	/**
	 * Self-multiplication operator.
	 * Rescale multiplication to reside in supported range.
	 * @param other Object to multiply with instance
	 * @return Reference to altered instance
	 */
	frac& operator*=(frac const& other)
	{
		if (m_data == Attr::min && other.m_data == Attr::min) {
			// becomes maximal positive value
			m_data = (Attr::min * Attr::min - 1) / (static_cast<double_precision_type>(scale()));
		} else {
			m_data = ((static_cast<double_precision_type>(m_data) *
			           static_cast<double_precision_type>(other.m_data))
			          << Attr::left) >>
			         Attr::right;
		}
		return *this;
	}

	/**
	 * Left shift operator.
	 * Saturation at minimal and maximal value.
	 * @param num_bits Number of bits to shift
	 * @return Reference to altered instance
	 */
	frac& operator<<=(int const num_bits)
	{
		double_precision_type tmp;
		tmp = static_cast<double_precision_type>(m_data) << num_bits;
		tmp = (tmp > Attr::max) ? Attr::max : tmp;
		tmp = (tmp < Attr::min) ? Attr::min : tmp;
		m_data = static_cast<data_type>(tmp);
		return *this;
	}

	/**
	 * Right shift operator.
	 * Saturation at minimal and maximal value.
	 * @param num_bits Number of bits to shift
	 * @return Reference to altered instance
	 */
	frac& operator>>=(int const num_bits)
	{
		double_precision_type tmp;
		tmp = static_cast<double_precision_type>(m_data) >> num_bits;
		tmp = (tmp > Attr::max) ? Attr::max : tmp;
		tmp = (tmp < Attr::min) ? Attr::min : tmp;
		m_data = static_cast<data_type>(tmp);
		return *this;
	}

	/**
	 * Addition operator.
	 * @param other Object to add to instance
	 * @return Object storing addition
	 */
	frac operator+(frac const& other) const
	{
		frac f = *this;
		f += other;
		return f;
	}

	/**
	 * Subtraction operator.
	 * @param other Object to subtract from instance
	 * @return Object storing subtraction
	 */
	frac operator-(frac const& other) const
	{
		frac f = *this;
		f -= other;
		return f;
	}

	/**
	 * Multiplication operator.
	 * @param other Object to multiply with instance
	 * @return Object storing multiplication
	 */
	frac operator*(frac const& other) const
	{
		frac f = *this;
		f *= other;
		return f;
	}

	/**
	 * Left shift operator.
	 * Saturation at minimal and maximal value.
	 * @param num_bits Number of bits to shift
	 * @return Object storing result
	 */
	frac operator<<(int const num_bits) const
	{
		frac f = *this;
		f <<= num_bits;
		return f;
	}

	/**
	 * Right shift operator.
	 * Saturation at minimal and maximal value.
	 * @param num_bits Number of bits to shift
	 * @return Object storing result
	 */
	frac operator>>(int const num_bits) const
	{
		frac f = *this;
		f >>= num_bits;
		return f;
	}

	/**
	 * Constant access to underlying integer data.
	 * @return Underlying integer
	 */
	data_type const& data() const { return m_data; }

	/**
	 * Reference access to underlying integer data.
	 * @return Underlying integer
	 */
	data_type& data() { return m_data; }

#ifndef __ppu__
	/**
	 * Print fractional number to ostream in the format ${integer value}/${scale}.
	 * @tparam Attr Attributes specializing fractional data type
	 * @param value Value to print
	 */
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, frac const& value)
	{
		os << static_cast<intmax_t>(value.data()) << "/" << value.scale();
		return os;
	}
#endif

private:
	/// Storage of number.
	data_type m_data;

	/**
	 * Helper for scale calculation.
	 * @param shift Exponent of power calculation
	 * @param value Start value, typically set to 1
	 * @return value * 2**shift
	 */
	constexpr static double_precision_type pow_2(
	    double_precision_type const shift, double_precision_type const value)
	{
		return (shift == 0) ? value : pow_2(shift - 1, 2 * value);
	}
};

/// 8-bit fractional data type specialization
template struct frac<detail::frac8_t_attributes>;
typedef frac<detail::frac8_t_attributes> frac8_t GENPYBIND(opaque(false), expose_as(frac8_t));

/// 16-bit fractional data type specialization
template struct frac<detail::frac16_t_attributes>;
typedef frac<detail::frac16_t_attributes> frac16_t GENPYBIND(opaque(false), expose_as(frac16_t));

#ifdef __ppu__
/**
 * Print fractional number to mailbox in the format ${integer value}/${scale}.
 * @tparam Attr Attributes specializing fractional data type
 * @param value Value to print
 */
template <class Attr>
void mailbox_write_frac(frac<Attr> const& value)
{
	using namespace libnux::vx;
	mailbox_write_signed_int(value.data());
	mailbox_write_string("/");
	mailbox_write_int(value.scale());
}
#endif

} // namespace libnux::vx
