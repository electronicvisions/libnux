#pragma once

#include "libnux/vx/mailbox.h"
#include "libnux/vx/spr.h"

#include <cstddef>
#include <cstdint>
#include <memory>

namespace libnux::vx {

using uart_data_rate_t = unsigned long;
using uart_word_t = uint16_t;


enum class Parity : uint8_t
{
	NONE,
	EVEN,
	ODD,
};

/**
 * Configuration of a UART interface.
 */
struct UartConfiguration
{
	/**
	 * Creates the configuration for a UART interface.
	 *
	 * @param width Width of the interface. Must not exceed the width of `uart_word_t`.
	 * @param parity Parity setting for the communication.
	 * @param stop_bits Amount of stop bits to send/expect.
	 */
	constexpr UartConfiguration(size_t const width, Parity const parity, size_t const stop_bits) :
	    width(width), parity(parity), stop_bits(stop_bits)
	{
		if (width > std::numeric_limits<uart_word_t>::digits) {
			mailbox_write_string("ERROR: UART width exceeds capacity of the underlying data type!");
			exit(1);
		}
	};

	size_t const width;
	Parity const parity;
	size_t const stop_bits;
};

constexpr UartConfiguration UART_8N1{8, Parity::NONE, 1};

/**
 * Software defined UART for transmitting serial data via the PPU's GPIO pin.
 */
class SoftUartTx
{
public:
	/**
	 * Creates a SoftUartTx instance.
	 *
	 * We eagerly allocate a buffer for printf-formatting to ensure that printing does not require
	 * additional excessive memory allocation.
	 *
	 * @param data_rate Data rate (in baud) to be used.
	 * @param config UART configuration, defaults to 8N1: 8bit, no parity, one stop bit.
	 * @param invert_physical Invert the signal on the line (idle 'low' if true).
	 * @param buffer_size Size of the string buffer (in byte) for printf formatting.
	 */
	explicit SoftUartTx(
	    uart_data_rate_t data_rate,
	    UartConfiguration const& config = UART_8N1,
	    bool invert_physical = false,
	    size_t buffer_size = 128);

	~SoftUartTx();

	/**
	 * Transmit a single word through the UART. Blocks until the transmission is done.
	 *
	 * @param data Data to be written.
	 */
	void write(uart_word_t data) const;

	/**
	 * Write a printf-formatted string via this UART. Blocks until the transmission is done.
	 *
	 * @param format A printf-style format string
	 * @return Number of bytes that resulted from formatting. In case of truncation, this number can
	 *         be larger than the `buffer_size` specified on construction. Can be negative in case
	 *         of formatting errors.
	 */
	int printf(char const* format, ...) const;

private:
	uart_data_rate_t const data_rate;
	time_base_t const cycles_per_bit;
	UartConfiguration const& config;
	bool const invert_physical;
	size_t const buffer_size;
	std::unique_ptr<char[]> string_buffer;
	[[nodiscard]] constexpr bool logic2physical(bool value) const;
};

} // namespace libnux::vx
