#pragma once
#include "libnux/vx/detail/vector_traits.h"
#include <cstddef>
#include <type_traits>

namespace libnux::vx {

/**
 * Parallel random number generator.
 *
 * It consists of an array of individual and independent random number generators, which are based
 * on the LFSR method with 16-bit state and generate 8-bit values. There exists one individual
 * generator per synapse column each with a different polynomial generating a different sequence
 * given the same initialization.
 * Conversely this means for each vector access, 128 of the total 256 random number generators are
 * accesses.
 */

/**
 * Set seed value of even or odd parallel random number generators accessible via the vector unit.
 * This replaces the 16-bit state per entry by eight high bits set to zero and the eight low bits
 * set from the given value.
 * When using a vector with 16-bit elements, their low and high bytes set individual rng entries.
 * @param value Value to set
 * @param index Index in range [0, 1] of parallel rng to set seed for.
 */
template <typename Vector>
std::enable_if_t<detail::IsVector<Vector>::value, void> parallel_rng_seed(
    Vector const& value, size_t index = 0);

/**
 * Set seed value of all parallel random number generators accessible via the vector unit.
 * This replaces the 16-bit state per entry by eight high bits set to zero and the eight low bits
 * set from the given value.
 * When using a vector row with 16-bit elements, their low and high bytes set individual rng
 * entries.
 * @param value Value to set
 */
template <typename VectorRow>
std::enable_if_t<
    detail::IsVectorHalfRow<VectorRow>::value ||
        (detail::IsVectorRow<VectorRow>::value && sizeof(VectorRow) == 256),
    void>
parallel_rng_seed(VectorRow const& value);

/**
 * Get random draw from parallel random number generator.
 * @param index Index in range [0, 1] of parallel rng to get value from (even (for 0) or odd (for 1)
 * 128 entries of the 256 random number generators).
 * @return (Pseudo) random value
 */
template <typename Vector>
std::enable_if_t<detail::IsVector<Vector>::value, Vector> parallel_rng_rand(size_t index);

/**
 * Get random draw from parallel random number generator.
 * For single vectors this defaults to using the rngs at index zero (even 128 entries of the 256
 * random number generators), for all other types all 256 random number generators are used.
 * @return (Pseudo) random value
 */
template <typename Vector>
std::enable_if_t<
    detail::IsVector<Vector>::value || detail::IsVectorHalfRow<Vector>::value ||
        detail::IsVectorRow<Vector>::value,
    Vector>
parallel_rng_rand();

} // namespace libnux::vx

#include "libnux/vx/parallel_rng.tcc"
