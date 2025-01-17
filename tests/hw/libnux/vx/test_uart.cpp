#include "libnux/vx/uart.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

auto uart = SoftUartTx(115'200, UART_8N1);

void test_uart_send_byte()
{
	testcase_begin("uart_send_byte");
	uart.write(0xAA);
	testcase_end();
}

void test_uart_send_string()
{
	testcase_begin("uart_send_string");
	size_t bytes_sent;

	bytes_sent = uart.printf("hello");
	test_equal(bytes_sent, 5u);

	bytes_sent = uart.printf("abc%d", 123);
	test_equal(bytes_sent, 6u);

	bytes_sent = uart.printf("%.2f", 3.1416);
	test_equal(bytes_sent, 4u);
	testcase_end();
}

void start()
{
	test_init();
	test_uart_send_byte();
	test_uart_send_string();
	test_summary();
	test_shutdown();
}
