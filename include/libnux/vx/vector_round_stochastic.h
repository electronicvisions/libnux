#pragma once
#include "libnux/vx/attrib.h"
#include "libnux/vx/detail/vector_traits.h"
#include <cstddef>
#include <type_traits>

namespace libnux::vx {

/**
 * Stochastically round the given number of lower bits of the elements of a vector.
 * This function uses the parallel random number generator accessible via the vector unit.
 * The given number of lower bits is set to zero after the operation, also when using saturating
 * arithmetics.
 * @param value Value to round
 * @param num_lsb Number of lower bits to round
 * @return Rounded value
 */
template <typename Vector>
std::enable_if_t<
    detail::IsVector<Vector>::value || detail::IsVectorHalfRow<Vector>::value ||
        detail::IsVectorRow<Vector>::value,
    Vector>
vector_round_stochastic(Vector const& value, size_t num_lsb);

} // namespace libnux::vx

#include "libnux/vx/vector_round_stochastic.tcc"
