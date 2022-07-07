#pragma once
#include "libnux/vx/vector_half_row_frac_sat_16.h"
#include "libnux/vx/vector_half_row_mod_16.h"

namespace libnux::vx {

VectorHalfRowMod16::VectorHalfRowMod16() : left(), right() {}

VectorHalfRowMod16::VectorHalfRowMod16(VectorHalfRowMod16::Element const value) :
    left(value), right(value)
{}

VectorHalfRowMod16::VectorHalfRowMod16(Quarter const& left, Quarter const& right) :
    left(left), right(right)
{}

VectorHalfRowMod16::VectorHalfRowMod16(VectorHalfRowMod16 const& other) :
    left(other.left), right(other.right)
{}

VectorHalfRowMod16& VectorHalfRowMod16::operator=(VectorHalfRowMod16 const& other)
{
	left = other.left;
	right = other.right;
	return *this;
}

VectorHalfRowMod16::Element& VectorHalfRowMod16::operator[](size_t const index)
{
	if (index < Quarter::size) {
		return left[index];
	} else {
		return right[index - Quarter::size];
	}
}

VectorHalfRowMod16::Element const& VectorHalfRowMod16::operator[](size_t const index) const
{
	if (index < Quarter::size) {
		return left[index];
	} else {
		return right[index - Quarter::size];
	}
}

bool VectorHalfRowMod16::operator==(VectorHalfRowMod16 const& other) const
{
	return (left == other.left) && (right == other.right);
}

bool VectorHalfRowMod16::operator!=(VectorHalfRowMod16 const& other) const
{
	return !(*this == other);
}

VectorHalfRowMod16 VectorHalfRowMod16::operator-() const
{
	return VectorHalfRowMod16(0) -= *this;
}

VectorHalfRowMod16& VectorHalfRowMod16::operator+=(VectorHalfRowMod16 const& rhs)
{
	left += rhs.left;
	right += rhs.right;
	return *this;
}

VectorHalfRowMod16& VectorHalfRowMod16::operator-=(VectorHalfRowMod16 const& rhs)
{
	left -= rhs.left;
	right -= rhs.right;
	return *this;
}

VectorHalfRowMod16& VectorHalfRowMod16::operator*=(VectorHalfRowMod16 const& rhs)
{
	left *= rhs.left;
	right *= rhs.right;
	return *this;
}

VectorHalfRowMod16& VectorHalfRowMod16::operator<<=(int const num_bits)
{
	left <<= num_bits;
	right <<= num_bits;
	return *this;
}

VectorHalfRowMod16& VectorHalfRowMod16::operator>>=(int const num_bits)
{
	left >>= num_bits;
	right >>= num_bits;
	return *this;
}

VectorHalfRowMod16 VectorHalfRowMod16::operator+(VectorHalfRowMod16 const& rhs) const
{
	VectorHalfRowMod16 ret(*this);
	return ret += rhs;
}

VectorHalfRowMod16 VectorHalfRowMod16::operator-(VectorHalfRowMod16 const& rhs) const
{
	VectorHalfRowMod16 ret(*this);
	return ret -= rhs;
}

VectorHalfRowMod16 VectorHalfRowMod16::operator*(VectorHalfRowMod16 const& rhs) const
{
	VectorHalfRowMod16 ret(*this);
	return ret *= rhs;
}

VectorHalfRowMod16 VectorHalfRowMod16::operator<<(int const num_bits) const
{
	VectorHalfRowMod16 ret(*this);
	return ret <<= num_bits;
}

VectorHalfRowMod16 VectorHalfRowMod16::operator>>(int const num_bits) const
{
	VectorHalfRowMod16 ret(*this);
	return ret >>= num_bits;
}

VectorHalfRowFracSat16 VectorHalfRowMod16::convert_contiguous() const
{
	return VectorHalfRowFracSat16(left.convert_contiguous(), right.convert_contiguous());
}

VectorHalfRowMod16::operator VectorHalfRowFracSat16() const
{
	return VectorHalfRowFracSat16(
	    static_cast<VectorFracSat16>(left), static_cast<VectorFracSat16>(right));
}

VectorHalfRowMod16 operator+(VectorHalfRowMod16::Element const lhs, VectorHalfRowMod16 const& rhs)
{
	return VectorHalfRowMod16(lhs) += rhs;
}

VectorHalfRowMod16 operator-(VectorHalfRowMod16::Element const lhs, VectorHalfRowMod16 const& rhs)
{
	return VectorHalfRowMod16(lhs) -= rhs;
}

VectorHalfRowMod16 operator*(VectorHalfRowMod16::Element const lhs, VectorHalfRowMod16 const& rhs)
{
	return VectorHalfRowMod16(lhs) *= rhs;
}

} // namespace libnux::vx
