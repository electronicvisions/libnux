#pragma once
#include "libnux/vx/dls.h"
#include "libnux/vx/parallel_rng.h"
#include "libnux/vx/detail/vector_traits.h"
#include <cstddef>
#include <type_traits>

namespace libnux::vx {

template <typename Vector>
std::enable_if_t<detail::IsVector<Vector>::value, void> parallel_rng_seed(
    Vector const& value, size_t index)
{
	asm volatile("fxvoutx %[value], %[base], %[index]\n"
	             :
	             : [value] "qv"(value.data), [base] "b"(dls_randgen_base), [index] "r"(index)
	             :);
}

template <typename VectorRow>
std::enable_if_t<
    detail::IsVectorHalfRow<VectorRow>::value ||
        (detail::IsVectorRow<VectorRow>::value && sizeof(VectorRow) == 256),
    void>
parallel_rng_seed(VectorRow const& value)
{
	if constexpr (detail::IsVectorHalfRow<VectorRow>::value) {
		parallel_rng_seed(value.left, 0);
		parallel_rng_seed(value.right, 1);
	} else {
		parallel_rng_seed(value.even, 0);
		parallel_rng_seed(value.odd, 1);
	}
}

template <typename Vector>
std::enable_if_t<detail::IsVector<Vector>::value, Vector> parallel_rng_rand(size_t index)
{
	Vector value;
	asm volatile("fxvinx %[value], %[base], %[index]\n"
	             : [value] "=qv"(value.data)
	             : [base] "b"(dls_randgen_base), [index] "r"(index)
	             :);
	return value;
}

template <typename Vector>
std::enable_if_t<
    detail::IsVector<Vector>::value || detail::IsVectorHalfRow<Vector>::value ||
        detail::IsVectorRow<Vector>::value,
    Vector>
parallel_rng_rand()
{
	if constexpr (detail::IsVector<Vector>::value) {
		return parallel_rng_rand<Vector>(0);
	} else if constexpr (detail::IsVectorHalfRow<Vector>::value) {
		return Vector(parallel_rng_rand<Vector::Quarter>(0)
		              , parallel_rng_rand<Vector::Quarter>(1));
	} else {
		if constexpr (detail::IsVectorHalfRow<typename Vector::HalfRow>::value) {
			return Vector(parallel_rng_rand<typename Vector::HalfRow>(), parallel_rng_rand<typename Vector::HalfRow>());
		} else {
			return Vector(
			    parallel_rng_rand<typename Vector::HalfRow>(0), parallel_rng_rand<typename Vector::HalfRow>(1));
		}
	}
}

} // namespace libnux::vx
