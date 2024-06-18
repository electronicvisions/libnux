#include <array>
#include <cstddef>
#include <cstdint>
#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

template <typename T, size_t BaseAddress = extmem_data_base>
void test_write_read(T const value, size_t const offset = (1 << 16) + 13)
{
	T* ptr = (T*) (BaseAddress + offset * sizeof(T));
	*ptr = value;
	// cf. issue #3739
	asm volatile("nop");
	test_equal(value, *ptr);
}

void start(void)
{
	test_init();

	testcase_begin("external memory write read via scalar unit");
	test_write_read<uint8_t>(123);
	test_write_read<uint16_t>(12345);
	test_write_read<uint32_t>(12345678);
	test_write_read<uint8_t>(17 + 4, 0x20000 - 1); // w/r highest extmem address
	testcase_end();

	testcase_begin("external dram memory write read via scalar unit");
	test_write_read<uint8_t, extmem_dram_data_base>(123);
	test_write_read<uint16_t, extmem_dram_data_base>(12345);
	test_write_read<uint32_t, extmem_dram_data_base>(12345678);
	test_write_read<uint8_t, extmem_dram_data_base>(
	    17 + 4, 0x10000000 - 1); // w/r highest extmem dram address
	testcase_end();

	test_summary();
	test_shutdown();
}
