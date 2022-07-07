#pragma once
#include "libnux/vx/vector_convert.h"
#include "libnux/vx/vector_frac_sat_16.h"
#include "libnux/vx/vector_mod_16.h"

namespace libnux::vx {

VectorMod16::VectorMod16() : data() {}

VectorMod16::VectorMod16(Element const value) : data(vec_splat_u16(value)) {}

VectorMod16::VectorMod16(Data const& value) : data(value) {}

VectorMod16::VectorMod16(VectorMod16 const& other) : data(other.data) {}

VectorMod16& VectorMod16::operator=(VectorMod16 const& other)
{
	data = other.data;
	return *this;
}

VectorMod16::Element& VectorMod16::operator[](size_t const index)
{
	return data[index];
}

VectorMod16::Element const& VectorMod16::operator[](size_t const index) const
{
	return data[index];
}

bool VectorMod16::operator==(VectorMod16 const& other) const
{
	bool ret = true;
	for (size_t i = 0; i < size; ++i) {
		ret = ret && (data[i] == other.data[i]);
	}
	return ret;
}

bool VectorMod16::operator!=(VectorMod16 const& other) const
{
	return !(*this == other);
}

VectorMod16 VectorMod16::operator-() const
{
	return VectorMod16(0) -= *this;
}

VectorMod16& VectorMod16::operator+=(VectorMod16 const& rhs)
{
	asm volatile("fxvaddhm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod16& VectorMod16::operator-=(VectorMod16 const& rhs)
{
	asm volatile("fxvsubhm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod16& VectorMod16::operator*=(VectorMod16 const& rhs)
{
	asm volatile("fxvmulhm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod16& VectorMod16::operator<<=(int const num_bits)
{
	switch (num_bits) {
		case 0: {
			break;
		}
#define VECTOR_SHIFT(NUM_BITS)                                                                     \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvshh %[data], %[data], %[num_bits]\n"                                      \
		             : [data] "+qv"(data)                                                          \
		             : [num_bits] "I"(NUM_BITS));                                                  \
		break;                                                                                     \
	}
			VECTOR_SHIFT(1)
			VECTOR_SHIFT(2)
			VECTOR_SHIFT(3)
			VECTOR_SHIFT(4)
			VECTOR_SHIFT(5)
			VECTOR_SHIFT(6)
			VECTOR_SHIFT(7)
			VECTOR_SHIFT(8)
			VECTOR_SHIFT(9)
			VECTOR_SHIFT(10)
			VECTOR_SHIFT(11)
			VECTOR_SHIFT(12)
			VECTOR_SHIFT(13)
			VECTOR_SHIFT(14)
			VECTOR_SHIFT(15)
			VECTOR_SHIFT(-1)
			VECTOR_SHIFT(-2)
			VECTOR_SHIFT(-3)
			VECTOR_SHIFT(-4)
			VECTOR_SHIFT(-5)
			VECTOR_SHIFT(-6)
			VECTOR_SHIFT(-7)
			VECTOR_SHIFT(-8)
			VECTOR_SHIFT(-9)
			VECTOR_SHIFT(-10)
			VECTOR_SHIFT(-11)
			VECTOR_SHIFT(-12)
			VECTOR_SHIFT(-13)
			VECTOR_SHIFT(-14)
			VECTOR_SHIFT(-15)
#undef VECTOR_SHIFT
		default: {
			data = vec_splat_u16(0);
		}
	}
	return *this;
}

VectorMod16& VectorMod16::operator>>=(int const num_bits)
{
	return this->operator<<=(-num_bits);
}

VectorMod16 VectorMod16::operator+(VectorMod16 const& rhs) const
{
	VectorMod16 ret(*this);
	return ret += rhs;
}

VectorMod16 VectorMod16::operator-(VectorMod16 const& rhs) const
{
	VectorMod16 ret(*this);
	return ret -= rhs;
}

VectorMod16 VectorMod16::operator*(VectorMod16 const& rhs) const
{
	VectorMod16 ret(*this);
	return ret *= rhs;
}

VectorMod16 VectorMod16::operator<<(int const num_bits) const
{
	VectorMod16 ret(*this);
	return ret <<= num_bits;
}

VectorMod16 VectorMod16::operator>>(int const num_bits) const
{
	VectorMod16 ret(*this);
	return ret >>= num_bits;
}

VectorFracSat16 VectorMod16::convert_contiguous() const
{
	return VectorFracSat16(uint16_to_int16(data));
}

VectorMod16::operator VectorFracSat16() const
{
	return VectorFracSat16(VectorFracSat16::Data(data));
}

VectorMod16 operator+(VectorMod16::Element const lhs, VectorMod16 const& rhs)
{
	return VectorMod16(lhs) += rhs;
}

VectorMod16 operator-(VectorMod16::Element const lhs, VectorMod16 const& rhs)
{
	return VectorMod16(lhs) -= rhs;
}

VectorMod16 operator*(VectorMod16::Element const lhs, VectorMod16 const& rhs)
{
	return VectorMod16(lhs) *= rhs;
}

} // namespace libnux::vx
