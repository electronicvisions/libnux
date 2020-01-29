#include "libnux/unittest.h"
#include "libnux/mailbox.h"

uint32_t libnux_test_write_string(char const * str) {
	return libnux_mailbox_write_string(str);
}

uint32_t libnux_test_write_int(uint32_t const n) {
	return libnux_mailbox_write_int(n);
}

void libnux_test_write_termination()
{
	libnux_mailbox_string_terminate();
}
