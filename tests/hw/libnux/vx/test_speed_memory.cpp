#include "libnux/vx/dls.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/time.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

void test_in_range(uint32_t value, uint32_t mean, uint32_t maxerr)
{
	test_true((value <= mean + maxerr) && (value >= mean - maxerr));
}

void test(char const* name, uint32_t mean, uint32_t maxerr, void (*measure)(uint32_t&, uint32_t&))
{
	testcase_begin(name);
	uint32_t t_pre;
	uint32_t t_post;
	measure(t_pre, t_post); // warm cache
	measure(t_pre, t_post);
	uint32_t const time_asm = t_post - t_pre - 1;
	test_in_range(time_asm, mean, maxerr);
	mailbox_write_int(time_asm);
	mailbox_write_string("\n");
	testcase_end();
}

void measure_extmem_load_fxvinx(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvinx 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_extmem_store_fxvoutx(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_extmem_store_fxvoutx_2(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"fxvoutx 1, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_extmem_store_fxvoutx_4(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"fxvoutx 1, %[base], %[index]\n"
		"fxvoutx 2, %[base], %[index]\n"
		"fxvoutx 3, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_extmem_store_fxvoutx_8(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"fxvoutx 1, %[base], %[index]\n"
		"fxvoutx 2, %[base], %[index]\n"
		"fxvoutx 3, %[base], %[index]\n"
		"fxvoutx 4, %[base], %[index]\n"
		"fxvoutx 5, %[base], %[index]\n"
		"fxvoutx 6, %[base], %[index]\n"
		"fxvoutx 7, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_extmem_store_fxvoutx_16(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"fxvoutx 1, %[base], %[index]\n"
		"fxvoutx 2, %[base], %[index]\n"
		"fxvoutx 3, %[base], %[index]\n"
		"fxvoutx 4, %[base], %[index]\n"
		"fxvoutx 5, %[base], %[index]\n"
		"fxvoutx 6, %[base], %[index]\n"
		"fxvoutx 7, %[base], %[index]\n"
		"fxvoutx 8, %[base], %[index]\n"
		"fxvoutx 9, %[base], %[index]\n"
		"fxvoutx 10, %[base], %[index]\n"
		"fxvoutx 11, %[base], %[index]\n"
		"fxvoutx 12, %[base], %[index]\n"
		"fxvoutx 13, %[base], %[index]\n"
		"fxvoutx 14, %[base], %[index]\n"
		"fxvoutx 15, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_extmem_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_intmem_store_fxvstax(uint32_t& t_pre, uint32_t& t_post)
{
	vector_type vec;
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvstax 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post),
		  "+m"(vec)
		: [base] "r" (&vec),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_intmem_load_fxvlax(uint32_t& t_pre, uint32_t& t_post)
{
	vector_type vec;
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvlax 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (&vec),
		  "m"(vec),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_weight_load(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvinx 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_weight_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_weight_store(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (dls_weight_base),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_neuron_reset(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvoutx 0, %[base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "r" (0x00c9'0000 + dls_num_columns / sizeof(uint32_t) * dls_num_rows),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

void measure_cadc_read(uint32_t& t_pre, uint32_t& t_post)
{
	// clang-format off
	asm volatile (
		"mfspr %[t_pre], 284\n"
		"fxvinx 0, %[base], %[index]\n"
		"fxvinx 1, %[buffered_base], %[index]\n"
		"sync\n"
		"mfspr %[t_post], 284\n"
		: [t_pre] "=&r" (t_pre),
		  [t_post] "=&r" (t_post)
		: [base] "b" (dls_causal_base),
		  [buffered_base] "b" ((dls_causal_base | dls_buffer_enable_mask) + 1),
		  [index] "r" (0)
		:
	);
	// clang-format on
}

int start()
{
	test_init();
	test("extmem store fxvoutx", 38, 4, measure_extmem_store_fxvoutx);
	// Disabled due to Issue: #4014
	// test("extmem store fxvoutx x2", 209, 18, measure_extmem_store_fxvoutx_2);
	//  FIXME (Issue #4006): More successive stores break the simulation (crc errors on link)
	//	test("extmem store fxvoutx x4", 306, 31, measure_extmem_store_fxvoutx_4);
	//	test("extmem store fxvoutx x8", 684, 69, measure_extmem_store_fxvoutx_8);
	//	test("extmem store fxvoutx x16", 1406, 141, measure_extmem_store_fxvoutx_16);
	// Disabled due to Issue: #4015
	// test("extmem load fxvinx", 344, 35, measure_extmem_load_fxvinx);
	test("intmem store fxvstax", 47, 5, measure_intmem_store_fxvstax);
	test("intmem load fxvlax", 49, 5, measure_intmem_load_fxvlax);
	test("weight store", 20, 2, measure_weight_store);
	test("weight load", 20, 2, measure_weight_load);
	test("neuron reset", 20, 2, measure_neuron_reset);
	test("cadc read", 410, 41, measure_cadc_read);
	test_summary();
	test_shutdown();
	return 0;
}
