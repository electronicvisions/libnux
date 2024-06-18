#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

uint32_t data_ext __attribute__((section("ext.data")));
uint32_t data_ext_dram __attribute__((section("ext_dram.data")));
uint32_t data_int;

void start()
{
	test_init();
	testcase_begin("data placement");
	test_true(reinterpret_cast<uint32_t>(&data_ext) & extmem_data_base);
	test_true(!(reinterpret_cast<uint32_t>(&data_int) & extmem_data_base));
	test_true(reinterpret_cast<uint32_t>(&data_ext_dram) & extmem_dram_data_base);
	test_true(!(reinterpret_cast<uint32_t>(&data_int) & extmem_dram_data_base));
	testcase_end();
	test_summary();
	test_shutdown();
}
