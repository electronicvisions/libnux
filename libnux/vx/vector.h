#pragma once

#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/omnibus.h"

namespace libnux::vx {

/**
 * One vector as processed by the PPU's vector unit.
 * A row in the chip's synapse memory or CADC may be composed by multiple of these.
 *
 * Empirical results show that these vectors need to be 32bit-word aligned for usage as
 * inputs/outputs in fxvinx/fxvoutx.
 */
typedef std::array<uint8_t, dls_vector_size> __attribute__((aligned(4))) vector_type;

/**
 * A single row in the chip's synapse memory or CADC.
 * On HICANN-Xv2, the vector unit can concurrently process half of all columns, divided in odd
 * columns and even columns.
 */
struct vector_row_t
{
	vector_type even_columns;
	vector_type odd_columns;

	bool operator==(vector_row_t const& rhs) const
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
		for (size_t i = 0; i < std::tuple_size<vector_type>::value; ++i) {
#pragma GCC diagnostic pop
			if (even_columns[i] != rhs.even_columns[i]) {
				return false;
			}
			if (odd_columns[i] != rhs.odd_columns[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(vector_row_t const& rhs) const { return !operator==(rhs); }

	void fill(uint8_t const value)
	{
		for (size_t i = 0; i < dls_vector_size; ++i) {
			even_columns[i] = value;
			odd_columns[i] = value;
		}
	}

	uint8_t const& operator[](size_t const i) const
	{
		return (i % 2) ? odd_columns[i / 2] : even_columns[i / 2];
	}

	uint8_t& operator[](size_t const i)
	{
		return (i % 2) ? odd_columns[i / 2] : even_columns[i / 2];
	}
};


/**
 * Read a complete row from the synapse memory or CADC in parallel via the vector unit.
 * @param row_id Row number to be read.
 * @param base Base address to be used, selects weight, address, correlation, ... readout.
 * @return Vector row that has been read back.
 */
inline vector_row_t get_row_via_vector(size_t const row_id, uint32_t const base)
{
	static constexpr uint32_t zero = 0;
	vector_row_t values;
	asm volatile(
	    // clang-format off
		"fxvinx 1, %[base], %[first_index]\n"
		"fxvinx 2, %[base], %[second_index]\n"
		"fxvstax 1, %[slindex], %[zero]\n"
		"fxvstax 2, %[srindex], %[zero]\n"
		"sync\n"
		: "+m"(const_cast<vector_type &>(values.even_columns)),
		"+m"(const_cast<vector_type &>(values.odd_columns))
		: [base] "b"(base),
		[first_index] "r"(row_id * 2),
		[second_index] "r"(row_id * 2 + 1),
		[slindex] "r"(values.even_columns.data()),
		[srindex] "r"(values.odd_columns.data()),
		[zero] "r"(zero)
		: /* no clobber */
	    // clang-format on
	);
	return values;
}


/**
 * Write a complete row to the synapse memory or CADC in parallel via the vector unit.
 * @param values Values to be written.
 * @param row_id Row number to be read.
 * @param base Base address to be used, selects weight, address, ... settings.
 */
inline void set_row_via_vector(vector_row_t const& values, size_t const row_id, uint32_t const base)
{
	static constexpr uint32_t zero = 0;
	asm volatile(
	    // clang-format off
		"fxvlax 1, %[slindex], %[zero]\n"
		"fxvlax 2, %[srindex], %[zero]\n"
		"fxvoutx 1, %[base], %[first_index]\n"
		"fxvoutx 2, %[base], %[second_index]\n"
		"sync\n"
		:
		: [base] "b"(base),
		[first_index] "r"(row_id * 2),
		[second_index] "r"(row_id * 2 + 1),
		[zero] "r"(zero),
				"m"(const_cast<vector_type &>(values.even_columns)),
				"m"(const_cast<vector_type &>(values.odd_columns)),
		[slindex] "r"(values.even_columns.data()),
		[srindex] "r"(values.odd_columns.data())
		: /* no clobber */
	    // clang-format on
	);
}

/**
 * Write a masked complete row to the synapse memory or CADC in parallel via the vector unit.
 * @param values Values to be written.
 * @param mask Mask to be used, in which values > 0 indicate usage, values <= 0 are discarded.
 * @param row_id Row number to be read.
 * @param base Base address to be used, selects weight, address, ... settings.
 */
inline void set_row_via_vector_masked(
    vector_row_t const& values, vector_row_t const& mask, size_t const row_id, uint32_t const base)
{
	static constexpr uint32_t zero = 0;
	asm volatile(
	    // clang-format off
		"fxvlax %3, %[mlindex], %[zero]\n"
		"fxvlax %4, %[mrindex], %[zero]\n"
		"fxvlax %1, %[slindex], %[zero]\n"
		"fxvlax %2, %[srindex], %[zero]\n"
		"fxvcmpb %3\n"
		"fxvoutx %1, %[base], %[first_index], 1\n"
		"fxvcmpb %4\n"
		"fxvoutx %2, %[base], %[second_index], 1\n"
		"sync\n"
		:
		: [base] "b" (base),
		  [first_index] "r" (row_id*2),
		  [second_index] "r" (row_id*2+1),
		  [slindex] "r" (values.even_columns.data()),
		  [srindex] "r" (values.odd_columns.data()),
		  [mlindex] "r" (mask.even_columns.data()),
		  [mrindex] "r" (mask.odd_columns.data()),
		  [zero] "r" (zero),
		  "m"(const_cast<vector_type &>(values.even_columns)),
		  "m"(const_cast<vector_type &>(values.odd_columns)),
		  "m"(const_cast<vector_type &>(mask.even_columns)),
		  "m"(const_cast<vector_type &>(mask.odd_columns))
		: /* no clobber */
	    // clang-format on
	);
}


/**
 * Read a complete row from the synapse memory or CADC serially via omnibus.
 * @param row_id Row number to be read.
 * @param base Base address to be used, selects weight, address, correlation, ... readout.
 * @return Vector row that has been read back.
 */
inline vector_row_t get_row_via_omnibus(size_t const row, uint32_t const base)
{
	vector_row_t ret;
	constexpr size_t num_words_per_row = dls_num_columns / sizeof(omnibus_word_t);
	constexpr size_t num_words_per_vector = dls_vector_size / sizeof(omnibus_word_t);
	auto const get_vector_via_omnibus = [](size_t const row, uint32_t const base, auto& columns) {
		for (size_t i = 0; i < num_words_per_row / 2; ++i) {
			omnibus_word_t const value = omnibus_read(base + i + row * num_words_per_row);
			uint8_t const* ptr = reinterpret_cast<uint8_t const*>(&value);
			for (size_t j = 0; j < sizeof(omnibus_word_t); ++j) {
				columns[i * sizeof(omnibus_word_t) + j] = ptr[j];
			}
		}
	};
	get_vector_via_omnibus(row, base, ret.even_columns);
	get_vector_via_omnibus(row, base + num_words_per_vector, ret.odd_columns);
	return ret;
}


vector_type get_vector(uint32_t base, uint32_t index)
{
	uint32_t zero = 0;
	vector_type values;
	asm volatile(
	    // clang-format off
		"fxvinx %1, %[base], %[index]\n"
		"fxvstax %1, %[sindex], %[zero]\n"
		"sync\n"
		: "+m"(const_cast<vector_type &>(values))
		: [base] "b" (base),
		  [index] "r" (index),
		  [sindex] "r" (values.data()),
		  [zero] "r" (zero)
		: /* no clobber */
	    // clang-format on
	);
	return values;
}

void set_vector(vector_type const& values, uint32_t base, uint32_t index)
{
	uint32_t zero = 0;
	asm volatile(
	    // clang-format off
		"fxvlax %1, %[sindex], %[zero]\n"
		"fxvoutx %1, %[base], %[index]\n"
		"sync\n"
		:
		: [base] "b" (base),
		  [index] "r" (index),
		  [sindex] "r" (values.data()),
		  [zero] "r" (zero),
		  "m"(const_cast<vector_type &>(values))
		: /* no clobber */
	    // clang-format on
	);
}

} // namespace libnux::vx
