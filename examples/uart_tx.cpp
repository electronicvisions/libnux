#include "libnux/vx/time.h"
#include "libnux/vx/uart.h"

using namespace libnux::vx;

auto uart = SoftUartTx(115'200, UART_8N1);

int start()
{
	sleep_cycles(100'000 * default_ppu_cycles_per_us);

	uart.printf("hello world");

	sleep_cycles(100'000 * default_ppu_cycles_per_us);
	return 0;
}
