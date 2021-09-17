#pragma once

#include <array>
#include <cstdint>

namespace libnux::vx {

// Returns the absolute difference of the two values
template <typename T>
T abs_diff(T input_1, T input_2)
{
	if (input_1 >= input_2) {
		return input_1 - input_2;
	}
	return input_2 - input_1;
}

// Returns the value in the array that occurs most
template <typename T, size_t Size>
T find_max_amount(std::array<T, Size> const& array)
{
	size_t n;
	size_t amount = 0;

	static_assert(Size > 0, "Array is empty");

	T max = array[0];

	for (size_t i = 0; i < Size - 1; i++) {
		n = 1;
		for (size_t j = i + 1; j < Size; j++) {
			n += (array[i] == array[j]);
		}
		if (n > amount) {
			amount = n;
			max = array[i];
		}
	}

	return max;
}

/**
 *  Prevent the compiler from optimizing away variables or expressions, even
 *  if the result is not used after calculation or known at compile time.
 *
 *  Combine with clobber() to keep variables alive and to keep stores.
 */
template <typename T>
inline void do_not_optimize_away(T const& p)
{
	asm volatile("" : : "g"(&p) : "memory");
}

/**
 *  Tell the compiler that all memory has been touched.
 *
 *  Example:
 *    ```cpp
 *    bool sth;
 *    // enforce existence
 *    do_not_optimize_away(sth);
 *    sth = false;
 *    // memory might have changed…
 *    clobber();
 *    // branch has to exist
 *    if (sth) { ... }
 *    ```
 */
inline void clobber()
{
	asm volatile("" : : : "memory");
}

} // namespace libnux::vx
