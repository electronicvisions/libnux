#pragma once
#include "libnux/vx/vector_frac_sat_8.h"
#include "libnux/vx/vector_mod_8.h"

namespace libnux::vx {

VectorFracSat8::VectorFracSat8() : data() {}

VectorFracSat8::VectorFracSat8(Element const value) : data(vec_splat_s8(value)) {}

VectorFracSat8::VectorFracSat8(Data const& value) : data(value) {}

VectorFracSat8::VectorFracSat8(VectorFracSat8 const& other) : data(other.data) {}

VectorFracSat8& VectorFracSat8::operator=(VectorFracSat8 const& other)
{
	data = other.data;
	return *this;
}

VectorFracSat8::Element& VectorFracSat8::operator[](size_t const index)
{
	return data[index];
}

VectorFracSat8::Element const& VectorFracSat8::operator[](size_t const index) const
{
	return data[index];
}

bool VectorFracSat8::operator==(VectorFracSat8 const& other) const
{
	bool ret = true;
	for (size_t i = 0; i < size; ++i) {
		ret = ret && (data[i] == other.data[i]);
	}
	return ret;
}

bool VectorFracSat8::operator!=(VectorFracSat8 const& other) const
{
	return !(*this == other);
}

VectorFracSat8 VectorFracSat8::operator-() const
{
	return VectorFracSat8(0) -= *this;
}

VectorFracSat8& VectorFracSat8::operator+=(VectorFracSat8 const& rhs)
{
	asm volatile("fxvaddbfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat8& VectorFracSat8::operator-=(VectorFracSat8 const& rhs)
{
	asm volatile("fxvsubbfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat8& VectorFracSat8::operator*=(VectorFracSat8 const& rhs)
{
	asm volatile("fxvmulbfs %[data], %[data], %[rhs]\n"
	             : [data] "+qv"(data)
	             : [rhs] "qv"(rhs.data));
	return *this;
}

VectorFracSat8& VectorFracSat8::operator<<=(int const num_bits)
{
	switch (num_bits) {
		case 0: {
			break;
		}
#define VECTOR_SHIFT_POS(NUM_BITS)                                                                 \
	case NUM_BITS: {                                                                               \
		Data sign;                                                                                 \
		asm volatile("fxvshb %[sign], %[data], -7\n"                                               \
		             "fxvshb %[sign], %[sign], 7\n"                                                \
		             "fxvshb %[data], %[data], %[num_bits]\n"                                      \
		             "fxvshb %[data], %[data], -1\n"                                               \
		             "fxvaddbm %[data], %[data], %[sign]\n"                                        \
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
#undef VECTOR_SHIFT_POS
#define VECTOR_SHIFT_NEG(NUM_BITS)                                                                 \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvmulbfs %[data], %[data], %[factor]\n"                                     \
		             : [data] "+qv"(data)                                                          \
		             : [factor] "qv"(vec_splat_s8(1 << (7 + NUM_BITS))));                          \
		break;                                                                                     \
	}
			VECTOR_SHIFT_NEG(-1)
			VECTOR_SHIFT_NEG(-2)
			VECTOR_SHIFT_NEG(-3)
			VECTOR_SHIFT_NEG(-4)
			VECTOR_SHIFT_NEG(-5)
			VECTOR_SHIFT_NEG(-6)
			VECTOR_SHIFT_NEG(-7)
#undef VECTOR_SHIFT_NEG
		default: {
			data = vec_splat_s8(0);
		}
	}
	return *this;
}

VectorFracSat8& VectorFracSat8::operator>>=(int const num_bits)
{
	return this->operator<<=(-num_bits);
}

VectorFracSat8 VectorFracSat8::operator+(VectorFracSat8 const& rhs) const
{
	VectorFracSat8 ret(*this);
	return ret += rhs;
}

VectorFracSat8 VectorFracSat8::operator-(VectorFracSat8 const& rhs) const
{
	VectorFracSat8 ret(*this);
	return ret -= rhs;
}

VectorFracSat8 VectorFracSat8::operator*(VectorFracSat8 const& rhs) const
{
	VectorFracSat8 ret(*this);
	return ret *= rhs;
}

VectorFracSat8 VectorFracSat8::operator<<(int const num_bits) const
{
	VectorFracSat8 ret(*this);
	return ret <<= num_bits;
}

VectorFracSat8 VectorFracSat8::operator>>(int const num_bits) const
{
	VectorFracSat8 ret(*this);
	return ret >>= num_bits;
}

VectorMod8 VectorFracSat8::convert_contiguous() const
{
	return VectorMod8(int8_to_uint8(data));
}

VectorFracSat8::operator VectorMod8() const
{
	return VectorMod8(VectorMod8::Data(data));
}

VectorFracSat8 operator+(VectorFracSat8::Element const lhs, VectorFracSat8 const& rhs)
{
	return VectorFracSat8(lhs) += rhs;
}

VectorFracSat8 operator-(VectorFracSat8::Element const lhs, VectorFracSat8 const& rhs)
{
	return VectorFracSat8(lhs) -= rhs;
}

VectorFracSat8 operator*(VectorFracSat8::Element const lhs, VectorFracSat8 const& rhs)
{
	return VectorFracSat8(lhs) *= rhs;
}

} // namespace libnux::vx
