#include "libnux/vx/unittest.h"
#include "libnux/vx/mailbox.h"

namespace libnux::vx {

uint32_t test_write_string(char const * str) {
	return mailbox_write_string(str);
}

uint32_t test_write_int(uint32_t const n) {
	return mailbox_write_int(n);
}

void test_write_termination()
{
	mailbox_string_terminate();
}

} // namespace libnux::vx
