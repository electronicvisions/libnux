#pragma once

#include <cstdint>

namespace libnux::vx {

typedef uint32_t omnibus_address_t;
typedef uint32_t omnibus_word_t;

/**
 * Get word-pointer to specified omnibus address.
 * @param address Omnibus address
 */
inline omnibus_word_t volatile* get_omnibus_pointer(omnibus_address_t const address)
{
	// Set high-bit in order to access Omnibus-from-PPU address range.
	constexpr omnibus_address_t omnibus_mask = 1ul << 31;
	// Multiply address by sizeof(word) in order to account for byte-addressing on PPU.
	// Omnibus address input is word addressing.
	return reinterpret_cast<omnibus_word_t*>((address * sizeof(omnibus_word_t)) | omnibus_mask);
}

/**
 * Read a word via Omnibus.
 * @param address Omnibus address to read from
 */
inline omnibus_word_t omnibus_read(omnibus_address_t const address)
{
	return *get_omnibus_pointer(address);
}

/**
 * Write a word via Omnibus.
 * @param address Omnibus address to write to
 * @param value Value to write
 */
inline void omnibus_write(omnibus_address_t const address, omnibus_word_t const value)
{
	*get_omnibus_pointer(address) = value;
}

} // namespace libnux::vx
