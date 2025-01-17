#include "libnux/vx/uart.h"
#include "libnux/vx/time.h"

#include <cstdarg>
#include <cstdio>
#include <vector>

namespace libnux::vx {

SoftUartTx::SoftUartTx(
    uart_data_rate_t const data_rate,
    UartConfiguration const& config,
    bool const invert_physical,
    size_t const buffer_size) :
    data_rate(data_rate),
    cycles_per_bit(default_ppu_cycles_per_us * 1'000'000 / data_rate),
    config(config),
    invert_physical(invert_physical),
    buffer_size(buffer_size),
    string_buffer(std::make_unique<char[]>(buffer_size))
{
	// configure output mode for GPIO
	set_goe(false); // inverted

	// default logic 'high'
	set_gout(logic2physical(true));
}

SoftUartTx::~SoftUartTx()
{
	// should not be necessary, cf. issue #4051
	string_buffer.reset();
}

constexpr bool SoftUartTx::logic2physical(bool const value) const
{
	if (invert_physical) {
		return (!value);
	}
	return (value);
}

void SoftUartTx::write(uart_word_t const data) const
{
	std::vector<bool> bits_to_write;

	// start bit
	bits_to_write.push_back(logic2physical(false));

	// data frame
	for (size_t bit_idx = 0; bit_idx < config.width; ++bit_idx) {
		// Check if the i-th bit is set
		const bool bit_value = (data & (1 << bit_idx)) != 0;
		bits_to_write.push_back(logic2physical(bit_value));
	}

	// (optional) parity bit
	switch (config.parity) {
		case Parity::NONE:
			break;
		case Parity::EVEN:
			bits_to_write.push_back(logic2physical(__builtin_parity(data)));
			break;
		case Parity::ODD:
			bits_to_write.push_back(logic2physical(!__builtin_parity(data)));
			break;
	}

	// stop bits
	for (size_t stop_bit = 0; stop_bit < config.stop_bits; ++stop_bit) {
		bits_to_write.push_back(logic2physical(true));
	}

	// warm cache
	sleep_cycles(10);
	set_gout(logic2physical(true));

	// write bits
	for (auto const value : bits_to_write) {
		set_gout(value);
		sleep_cycles(cycles_per_bit);
	}
}

int SoftUartTx::printf(char const* const format, ...) const
{
	va_list args;

	va_start(args, format);
	const int retval = vsnprintf(string_buffer.get(), buffer_size, format, args);
	va_end(args);

	if (retval < 0) {
		// formatting failed, don't output anything
		return retval;
	}

	char const* encoded = string_buffer.get();
	while (*encoded != '\0') {
		write(*encoded);
		++encoded;
	}

	return retval;
}

} // namespace libnux::vx
