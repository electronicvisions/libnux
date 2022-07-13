#pragma once
#include "libnux/vx/vector_row.h"
#include <utility>

namespace libnux::vx {

template <typename HalfRow>
VectorRow<HalfRow>::VectorRow(VectorRow<HalfRow>::Element const value) : even(value), odd(value)
{}

template <typename HalfRow>
VectorRow<HalfRow>::VectorRow(HalfRow const& even, HalfRow const& odd) : even(even), odd(odd)
{}

template <typename HalfRow>
VectorRow<HalfRow>::VectorRow(VectorRow const& other) : even(other.even), odd(other.odd)
{}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator=(VectorRow const& other)
{
	even = other.even;
	odd = other.odd;
	return *this;
}

template <typename HalfRow>
typename VectorRow<HalfRow>::Element& VectorRow<HalfRow>::operator[](size_t const index)
{
	return (index % 2) ? odd[index / 2] : even[index / 2];
}

template <typename HalfRow>
typename VectorRow<HalfRow>::Element const& VectorRow<HalfRow>::operator[](size_t const index) const
{
	return (index % 2) ? odd[index / 2] : even[index / 2];
}

template <typename HalfRow>
bool VectorRow<HalfRow>::operator==(VectorRow const& other) const
{
	return (even == other.even) && (odd == other.odd);
}

template <typename HalfRow>
bool VectorRow<HalfRow>::operator!=(VectorRow const& other) const
{
	return !(*this == other);
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator-() const
{
	return VectorRow(0) -= *this;
}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator+=(VectorRow const& rhs)
{
	even += rhs.even;
	odd += rhs.odd;
	return *this;
}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator-=(VectorRow const& rhs)
{
	even -= rhs.even;
	odd -= rhs.odd;
	return *this;
}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator*=(VectorRow const& rhs)
{
	even *= rhs.even;
	odd *= rhs.odd;
	return *this;
}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator<<=(int const num_bits)
{
	even <<= num_bits;
	odd <<= num_bits;
	return *this;
}

template <typename HalfRow>
VectorRow<HalfRow>& VectorRow<HalfRow>::operator>>=(int const num_bits)
{
	even >>= num_bits;
	odd >>= num_bits;
	return *this;
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator+(VectorRow const& rhs) const
{
	VectorRow ret(*this);
	return ret += rhs;
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator-(VectorRow const& rhs) const
{
	VectorRow ret(*this);
	return ret -= rhs;
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator*(VectorRow const& rhs) const
{
	VectorRow ret(*this);
	return ret *= rhs;
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator<<(int const num_bits) const
{
	VectorRow ret(*this);
	return ret <<= num_bits;
}

template <typename HalfRow>
VectorRow<HalfRow> VectorRow<HalfRow>::operator>>(int const num_bits) const
{
	VectorRow ret(*this);
	return ret >>= num_bits;
}

template <typename HalfRow>
VectorRow<decltype(std::declval<HalfRow>().convert_contiguous())>
VectorRow<HalfRow>::convert_contiguous() const
{
	return VectorRow<decltype(std::declval<HalfRow>().convert_contiguous())>(
	    even.convert_contiguous(), odd.convert_contiguous());
}

template <>
template <>
VectorRowMod8::operator VectorRowFracSat8() const
{
	return VectorRowFracSat8(
	    static_cast<VectorHalfRowFracSat8>(even),
	    static_cast<VectorHalfRowFracSat8>(odd));
}

template <>
template <>
VectorRowMod16::operator VectorRowFracSat16() const
{
	return VectorRowFracSat16(
	    static_cast<VectorHalfRowFracSat16>(even),
	    static_cast<VectorHalfRowFracSat16>(odd));
}

template <>
template <>
VectorRowFracSat8::operator VectorRowMod8() const
{
	return VectorRowMod8(
	    static_cast<VectorHalfRowMod8>(even), static_cast<VectorHalfRowMod8>(odd));
}

template <>
template <>
VectorRowFracSat16::operator VectorRowMod16() const
{
	return VectorRowMod16(
	    static_cast<VectorHalfRowMod16>(even), static_cast<VectorHalfRowMod16>(odd));
}

template <typename HalfRow>
VectorRow<HalfRow> operator+(
    typename VectorRow<HalfRow>::Element const lhs, VectorRow<HalfRow> const& rhs)
{
	return VectorRow<HalfRow>(lhs) += rhs;
}

template <typename HalfRow>
VectorRow<HalfRow> operator-(
    typename VectorRow<HalfRow>::Element const lhs, VectorRow<HalfRow> const& rhs)
{
	return VectorRow<HalfRow>(lhs) -= rhs;
}

template <typename HalfRow>
VectorRow<HalfRow> operator*(
    typename VectorRow<HalfRow>::Element const lhs, VectorRow<HalfRow> const& rhs)
{
	return VectorRow<HalfRow>(lhs) *= rhs;
}

} // namespace libnux::vx
