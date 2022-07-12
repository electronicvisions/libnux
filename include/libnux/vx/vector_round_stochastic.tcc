#pragma once
#include "libnux/vx/detail/vector_traits.h"
#include "libnux/vx/parallel_rng.h"
#include "libnux/vx/vector_round_stochastic.h"
#include <type_traits>

namespace libnux::vx {

template <typename Vector>
std::enable_if_t<
    detail::IsVector<Vector>::value || detail::IsVectorHalfRow<Vector>::value ||
        detail::IsVectorRow<Vector>::value,
    Vector>
vector_round_stochastic(Vector const& value, size_t const num_lsb)
{
	if (num_lsb == 0) {
		return value;
	}
	if constexpr (detail::IsVector<Vector>::value) {
		Vector tmp = parallel_rng_rand<Vector>();
		if constexpr (sizeof(typename Vector::Element) == 1) { // int8_t / uint8_t value type
#define VECTOR_ROUND_STOCHASTIC_IMPL(NUM_BITS)                                                     \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvshb %[tmp], %[tmp], %[truncate_shift]\n"                                  \
		             : [tmp] "+&qv"(tmp.data)                                                      \
		             : [truncate_shift] "I"(-(8 - NUM_BITS)));                                     \
		tmp += value;                                                                              \
		asm volatile("fxvshb %[tmp], %[tmp], %[neg_truncate_shift]\n"                              \
		             "fxvshb %[tmp], %[tmp], %[truncate_shift]\n"                                  \
		             : [tmp] "+&qv"(tmp.data)                                                      \
		             : [truncate_shift] "I"(NUM_BITS), [neg_truncate_shift] "I"(-NUM_BITS));       \
		break;                                                                                     \
	}
			switch (num_lsb) {
				VECTOR_ROUND_STOCHASTIC_IMPL(1)
				VECTOR_ROUND_STOCHASTIC_IMPL(2)
				VECTOR_ROUND_STOCHASTIC_IMPL(3)
				VECTOR_ROUND_STOCHASTIC_IMPL(4)
				VECTOR_ROUND_STOCHASTIC_IMPL(5)
				VECTOR_ROUND_STOCHASTIC_IMPL(6)
				VECTOR_ROUND_STOCHASTIC_IMPL(7)
				default: {
					return Vector(0);
				}
			}
#undef VECTOR_ROUND_STOCHASTIC_IMPL
		} else { // int16_t / uint16_t value type
			static_assert(sizeof(typename Vector::Element) == 2);
#define VECTOR_ROUND_STOCHASTIC_IMPL(NUM_BITS)                                                     \
	case NUM_BITS: {                                                                               \
		asm volatile("fxvshh %[tmp], %[tmp], %[truncate_shift]\n"                                  \
		             : [tmp] "+&qv"(tmp.data)                                                      \
		             : [truncate_shift] "I"(-(16 - NUM_BITS)));                                    \
		tmp += value;                                                                              \
		asm volatile("fxvshh %[tmp], %[tmp], %[neg_truncate_shift]\n"                              \
		             "fxvshh %[tmp], %[tmp], %[truncate_shift]\n"                                  \
		             : [tmp] "+&qv"(tmp.data)                                                      \
		             : [truncate_shift] "I"(NUM_BITS), [neg_truncate_shift] "I"(-NUM_BITS));       \
		break;                                                                                     \
	}
			switch (num_lsb) {
				VECTOR_ROUND_STOCHASTIC_IMPL(1)
				VECTOR_ROUND_STOCHASTIC_IMPL(2)
				VECTOR_ROUND_STOCHASTIC_IMPL(3)
				VECTOR_ROUND_STOCHASTIC_IMPL(4)
				VECTOR_ROUND_STOCHASTIC_IMPL(5)
				VECTOR_ROUND_STOCHASTIC_IMPL(6)
				VECTOR_ROUND_STOCHASTIC_IMPL(7)
				VECTOR_ROUND_STOCHASTIC_IMPL(8)
				VECTOR_ROUND_STOCHASTIC_IMPL(9)
				VECTOR_ROUND_STOCHASTIC_IMPL(10)
				VECTOR_ROUND_STOCHASTIC_IMPL(11)
				VECTOR_ROUND_STOCHASTIC_IMPL(12)
				VECTOR_ROUND_STOCHASTIC_IMPL(13)
				VECTOR_ROUND_STOCHASTIC_IMPL(14)
				VECTOR_ROUND_STOCHASTIC_IMPL(15)
				default: {
					return Vector(0);
				}
			}
#undef VECTOR_ROUND_STOCHASTIC_IMPL
		}
		return tmp;
	} else if constexpr (detail::IsVectorHalfRow<Vector>::value) {
		return Vector(
		    vector_round_stochastic(value.left, num_lsb),
		    vector_round_stochastic(value.right, num_lsb));
	} else {
		return Vector(
		    vector_round_stochastic(value.even, num_lsb),
		    vector_round_stochastic(value.odd, num_lsb));
	}
}

} // namespace libnux::vx
