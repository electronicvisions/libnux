#pragma once
#include "libnux/vx/vector_convert.h"
#include "libnux/vx/vector_frac_sat_8.h"
#include "libnux/vx/vector_mod_8.h"

namespace libnux::vx {

VectorMod8::VectorMod8() : data() {}

VectorMod8::VectorMod8(Element const value) : data(vec_splat_u8(value)) {}

VectorMod8::VectorMod8(Data const& value) : data(value) {}

VectorMod8::VectorMod8(VectorMod8 const& other) : data(other.data) {}

VectorMod8& VectorMod8::operator=(VectorMod8 const& other)
{
	data = other.data;
	return *this;
}

VectorMod8::Element& VectorMod8::operator[](size_t const index)
{
	return data[index];
}

VectorMod8::Element const& VectorMod8::operator[](size_t const index) const
{
	return data[index];
}

bool VectorMod8::operator==(VectorMod8 const& other) const
{
	bool ret = true;
	for (size_t i = 0; i < size; ++i) {
		ret = ret && (data[i] == other.data[i]);
	}
	return ret;
}

bool VectorMod8::operator!=(VectorMod8 const& other) const
{
	return !(*this == other);
}

VectorMod8 VectorMod8::operator-() const
{
	return VectorMod8(0) -= *this;
}

VectorMod8& VectorMod8::operator+=(VectorMod8 const& rhs)
{
	asm volatile("fxvaddbm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod8& VectorMod8::operator-=(VectorMod8 const& rhs)
{
	asm volatile("fxvsubbm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod8& VectorMod8::operator*=(VectorMod8 const& rhs)
{
	asm volatile("fxvmulbm %[data], %[data], %[rhs]\n" : [data] "+qv"(data) : [rhs] "qv"(rhs.data));
	return *this;
}

VectorMod8& VectorMod8::operator<<=(int const num_bits)
{
	switch (num_bits) {
		case 0: {
			break;
		}
#define VECTOR_SHIFT(NUM_BITS)                                                                     \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvshb %[data], %[data], %[num_bits]\n"                                      \
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
			VECTOR_SHIFT(-1)
			VECTOR_SHIFT(-2)
			VECTOR_SHIFT(-3)
			VECTOR_SHIFT(-4)
			VECTOR_SHIFT(-5)
			VECTOR_SHIFT(-6)
			VECTOR_SHIFT(-7)
#undef VECTOR_SHIFT
		default: {
			data = vec_splat_u8(0);
		}
	}
	return *this;
}

VectorMod8& VectorMod8::operator>>=(int const num_bits)
{
	return this->operator<<=(-num_bits);
}

VectorMod8 VectorMod8::operator+(VectorMod8 const& rhs) const
{
	VectorMod8 ret(*this);
	return ret += rhs;
}

VectorMod8 VectorMod8::operator-(VectorMod8 const& rhs) const
{
	VectorMod8 ret(*this);
	return ret -= rhs;
}

VectorMod8 VectorMod8::operator*(VectorMod8 const& rhs) const
{
	VectorMod8 ret(*this);
	return ret *= rhs;
}

VectorMod8 VectorMod8::operator<<(int const num_bits) const
{
	VectorMod8 ret(*this);
	return ret <<= num_bits;
}

VectorMod8 VectorMod8::operator>>(int const num_bits) const
{
	VectorMod8 ret(*this);
	return ret >>= num_bits;
}

VectorFracSat8 VectorMod8::convert_contiguous() const
{
	return VectorFracSat8(uint8_to_int8(data));
}

VectorMod8::operator VectorFracSat8() const
{
	return VectorFracSat8(VectorFracSat8::Data(data));
}

VectorMod8 operator+(VectorMod8::Element const lhs, VectorMod8 const& rhs)
{
	return VectorMod8(lhs) += rhs;
}

VectorMod8 operator-(VectorMod8::Element const lhs, VectorMod8 const& rhs)
{
	return VectorMod8(lhs) -= rhs;
}

VectorMod8 operator*(VectorMod8::Element const lhs, VectorMod8 const& rhs)
{
	return VectorMod8(lhs) *= rhs;
}

} // namespace libnux::vx
