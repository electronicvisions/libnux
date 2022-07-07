#pragma once
#include "libnux/vx/vector_convert.h"
#include "libnux/vx/vector_frac_sat_16.h"
#include "libnux/vx/vector_mod_16.h"

namespace libnux::vx {

VectorFracSat16::VectorFracSat16() : data() {}

VectorFracSat16::VectorFracSat16(Element const value) : data(vec_splat_s16(value)) {}

VectorFracSat16::VectorFracSat16(Data const& value) : data(value) {}

VectorFracSat16::VectorFracSat16(VectorFracSat16 const& other) : data(other.data) {}

VectorFracSat16& VectorFracSat16::operator=(VectorFracSat16 const& other)
{
	data = other.data;
	return *this;
}

VectorFracSat16::Element& VectorFracSat16::operator[](size_t const index)
{
	return data[index];
}

VectorFracSat16::Element const& VectorFracSat16::operator[](size_t const index) const
{
	return data[index];
}

bool VectorFracSat16::operator==(VectorFracSat16 const& other) const
{
	bool ret = true;
	for (size_t i = 0; i < size; ++i) {
		ret = ret && (data[i] == other.data[i]);
	}
	return ret;
}

bool VectorFracSat16::operator!=(VectorFracSat16 const& other) const
{
	return !(*this == other);
}

VectorFracSat16 VectorFracSat16::operator-() const
{
	return VectorFracSat16(0) -= *this;
}

VectorFracSat16& VectorFracSat16::operator+=(VectorFracSat16 const& rhs)
{
	asm volatile("fxvaddhfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat16& VectorFracSat16::operator-=(VectorFracSat16 const& rhs)
{
	asm volatile("fxvsubhfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat16& VectorFracSat16::operator*=(VectorFracSat16 const& rhs)
{
	asm volatile("fxvmulhfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat16& VectorFracSat16::operator<<=(int const num_bits)
{
	switch (num_bits) {
		case 0: {
			break;
		}
#define VECTOR_SHIFT_POS(NUM_BITS)                                                                 \
	case NUM_BITS: {                                                                               \
		Data sign;                                                                                 \
		asm volatile("fxvshh %[sign], %[data], -15\n"                                              \
		             "fxvshh %[sign], %[sign], 15\n"                                               \
		             "fxvshh %[data], %[data], %[num_bits]\n"                                      \
		             "fxvshh %[data], %[data], -1\n"                                               \
		             "fxvaddhm %[data], %[data], %[sign]\n"                                        \
		             : [sign] "=&qv"(sign), [data] "+&qv"(data)                                    \
		             : [num_bits] "I"(NUM_BITS + 1));                                              \
		break;                                                                                     \
	}
			VECTOR_SHIFT_POS(1)
			VECTOR_SHIFT_POS(2)
			VECTOR_SHIFT_POS(3)
			VECTOR_SHIFT_POS(4)
			VECTOR_SHIFT_POS(5)
			VECTOR_SHIFT_POS(6)
			VECTOR_SHIFT_POS(7)
			VECTOR_SHIFT_POS(8)
			VECTOR_SHIFT_POS(9)
			VECTOR_SHIFT_POS(10)
			VECTOR_SHIFT_POS(11)
			VECTOR_SHIFT_POS(12)
			VECTOR_SHIFT_POS(13)
			VECTOR_SHIFT_POS(14)
			VECTOR_SHIFT_POS(15)
#undef VECTOR_SHIFT_POS
#define VECTOR_SHIFT_NEG(NUM_BITS)                                                                 \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvmulhfs %[data], %[data], %[factor]\n"                                     \
		             : [data] "+qv"(data)                                                          \
		             : [factor] "qv"(vec_splat_s16(1 << (15 + NUM_BITS))));                        \
		break;                                                                                     \
	}
			VECTOR_SHIFT_NEG(-1)
			VECTOR_SHIFT_NEG(-2)
			VECTOR_SHIFT_NEG(-3)
			VECTOR_SHIFT_NEG(-4)
			VECTOR_SHIFT_NEG(-5)
			VECTOR_SHIFT_NEG(-6)
			VECTOR_SHIFT_NEG(-7)
			VECTOR_SHIFT_NEG(-8)
			VECTOR_SHIFT_NEG(-9)
			VECTOR_SHIFT_NEG(-10)
			VECTOR_SHIFT_NEG(-11)
			VECTOR_SHIFT_NEG(-12)
			VECTOR_SHIFT_NEG(-13)
			VECTOR_SHIFT_NEG(-14)
			VECTOR_SHIFT_NEG(-15)
#undef VECTOR_SHIFT_NEG
		default: {
			data = vec_splat_s16(0);
		}
	}
	return *this;
}

VectorFracSat16& VectorFracSat16::operator>>=(int const num_bits)
{
	return this->operator<<=(-num_bits);
}

VectorFracSat16 VectorFracSat16::operator+(VectorFracSat16 const& rhs) const
{
	VectorFracSat16 ret(*this);
	return ret += rhs;
}

VectorFracSat16 VectorFracSat16::operator-(VectorFracSat16 const& rhs) const
{
	VectorFracSat16 ret(*this);
	return ret -= rhs;
}

VectorFracSat16 VectorFracSat16::operator*(VectorFracSat16 const& rhs) const
{
	VectorFracSat16 ret(*this);
	return ret *= rhs;
}

VectorFracSat16 VectorFracSat16::operator<<(int const num_bits) const
{
	VectorFracSat16 ret(*this);
	return ret <<= num_bits;
}

VectorFracSat16 VectorFracSat16::operator>>(int const num_bits) const
{
	VectorFracSat16 ret(*this);
	return ret >>= num_bits;
}

VectorMod16 VectorFracSat16::convert_contiguous() const
{
	return VectorMod16(int16_to_uint16(data));
}

VectorFracSat16::operator VectorMod16() const
{
	return VectorMod16(VectorMod16::Data(data));
}

VectorFracSat16 operator+(VectorFracSat16::Element const lhs, VectorFracSat16 const& rhs)
{
	return VectorFracSat16(lhs) += rhs;
}

VectorFracSat16 operator-(VectorFracSat16::Element const lhs, VectorFracSat16 const& rhs)
{
	return VectorFracSat16(lhs) -= rhs;
}

VectorFracSat16 operator*(VectorFracSat16::Element const lhs, VectorFracSat16 const& rhs)
{
	return VectorFracSat16(lhs) *= rhs;
}

} // namespace libnux::vx
