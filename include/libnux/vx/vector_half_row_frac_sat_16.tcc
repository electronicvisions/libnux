#pragma once
#include "libnux/vx/vector_half_row_frac_sat_16.h"
#include "libnux/vx/vector_half_row_mod_16.h"

namespace libnux::vx {

VectorHalfRowFracSat16::VectorHalfRowFracSat16() : left(), right() {}

VectorHalfRowFracSat16::VectorHalfRowFracSat16(VectorHalfRowFracSat16::Element const value) :
    left(value), right(value)
{}

VectorHalfRowFracSat16::VectorHalfRowFracSat16(Quarter const& left, Quarter const& right) :
    left(left), right(right)
{}

VectorHalfRowFracSat16::VectorHalfRowFracSat16(VectorHalfRowFracSat16 const& other) :
    left(other.left), right(other.right)
{}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator=(VectorHalfRowFracSat16 const& other)
{
	left = other.left;
	right = other.right;
	return *this;
}

VectorHalfRowFracSat16::Element& VectorHalfRowFracSat16::operator[](size_t const index)
{
	if (index < Quarter::size) {
		return left[index];
	} else {
		return right[index - Quarter::size];
	}
}

VectorHalfRowFracSat16::Element const& VectorHalfRowFracSat16::operator[](size_t const index) const
{
	if (index < Quarter::size) {
		return left[index];
	} else {
		return right[index - Quarter::size];
	}
}

bool VectorHalfRowFracSat16::operator==(VectorHalfRowFracSat16 const& other) const
{
	return (left == other.left) && (right == other.right);
}

bool VectorHalfRowFracSat16::operator!=(VectorHalfRowFracSat16 const& other) const
{
	return !(*this == other);
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator-() const
{
	return VectorHalfRowFracSat16(0) -= *this;
}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator+=(VectorHalfRowFracSat16 const& rhs)
{
	left += rhs.left;
	right += rhs.right;
	return *this;
}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator-=(VectorHalfRowFracSat16 const& rhs)
{
	left -= rhs.left;
	right -= rhs.right;
	return *this;
}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator*=(VectorHalfRowFracSat16 const& rhs)
{
	left *= rhs.left;
	right *= rhs.right;
	return *this;
}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator<<=(int const num_bits)
{
	left <<= num_bits;
	right <<= num_bits;
	return *this;
}

VectorHalfRowFracSat16& VectorHalfRowFracSat16::operator>>=(int const num_bits)
{
	left >>= num_bits;
	right >>= num_bits;
	return *this;
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator+(VectorHalfRowFracSat16 const& rhs) const
{
	VectorHalfRowFracSat16 ret(*this);
	return ret += rhs;
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator-(VectorHalfRowFracSat16 const& rhs) const
{
	VectorHalfRowFracSat16 ret(*this);
	return ret -= rhs;
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator*(VectorHalfRowFracSat16 const& rhs) const
{
	VectorHalfRowFracSat16 ret(*this);
	return ret *= rhs;
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator<<(int const num_bits) const
{
	VectorHalfRowFracSat16 ret(*this);
	return ret <<= num_bits;
}

VectorHalfRowFracSat16 VectorHalfRowFracSat16::operator>>(int const num_bits) const
{
	VectorHalfRowFracSat16 ret(*this);
	return ret >>= num_bits;
}

VectorHalfRowMod16 VectorHalfRowFracSat16::convert_contiguous() const
{
	return VectorHalfRowMod16(left.convert_contiguous(), right.convert_contiguous());
}

VectorHalfRowFracSat16::operator VectorHalfRowMod16() const
{
	return VectorHalfRowMod16(static_cast<VectorMod16>(left), static_cast<VectorMod16>(right));
}

VectorHalfRowFracSat16 operator+(
    VectorHalfRowFracSat16::Element const lhs, VectorHalfRowFracSat16 const& rhs)
{
	return VectorHalfRowFracSat16(lhs) += rhs;
}

VectorHalfRowFracSat16 operator-(
    VectorHalfRowFracSat16::Element const lhs, VectorHalfRowFracSat16 const& rhs)
{
	return VectorHalfRowFracSat16(lhs) -= rhs;
}

VectorHalfRowFracSat16 operator*(
    VectorHalfRowFracSat16::Element const lhs, VectorHalfRowFracSat16 const& rhs)
{
	return VectorHalfRowFracSat16(lhs) *= rhs;
}

} // namespace libnux::vx
