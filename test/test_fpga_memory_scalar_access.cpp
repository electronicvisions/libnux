#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/dls.h"
#include "libnux/unittest.h"

using namespace libnux;

template <typename T>
void test_write_read(T const value, size_t const offset = 13)
{
	T* ptr = (T*) (extmem_data_base + offset * sizeof(T));
	*ptr = value;
	// cf. issue #3739
	asm volatile("nop");
	libnux_test_equal(value, *ptr);
}

void start(void)
{
	libnux_test_init();

	libnux_testcase_begin("external memory write read via scalar unit");
	test_write_read<uint8_t>(123);
	test_write_read<uint16_t>(12345);
	test_write_read<uint32_t>(12345678);
	libnux_testcase_end();

	libnux_test_summary();
	libnux_test_shutdown();
}
