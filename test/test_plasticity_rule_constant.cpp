#include <s2pp.h>
#include "libnux/Mask.hpp"
#include "libnux/PlasticityRule_Constant.hpp"
#include "libnux/dls.h"
#include "libnux/unittest.h"

void test_plasticity_rule_constant()
{
	libnux_testcase_begin("plasticity_rule_constant");

	// generate masks
	__vector uint8_t mask1 = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	__vector uint8_t mask2 = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	Mask<1, 1> m1;
	Mask<1, 0> m2;
	m1.partial_vector_synram_address[0] = 0;
	m1.full_vector_synram_address[0] = 1;
	m2.partial_vector_synram_address[0] = 0;
	m1.vectors[0] = mask1;
	m2.vectors[0] = mask2;

	// initialize plasticity rules
	auto rule1 = PlasticityRules::Constant<Mask<1, 1>>(m1, 2);
	auto rule2 = PlasticityRules::Constant<Mask<1, 0>>(m2, 3);

	// write weights masked
	rule1.run();
	rule2.run();

	// read and test written weights
	uint32_t i = 0;
	uint32_t j = 1;
	__vector uint8_t read0, read1;
	// clang-format off
	asm volatile(
		"sync\n"
		"fxvinx %[read0], %[dls_weight_base], %[i]\n"
		"fxvinx %[read1], %[dls_weight_base], %[j]\n"
		"sync\n"
		: [read0] "=kv"(read0), [read1] "=kv"(read1)
		: [dls_weight_base] "b"(dls_weight_base), [i] "r"(i), [j] "r"(j)
		:);
	// clang-format on
	__vector uint8_t expected_partial = {2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	for (uint32_t i = 0; i < 16; ++i) {
		libnux_test_equal(read0[i], expected_partial[i]);
		libnux_test_equal(read1[i], 2);
	}

	libnux_testcase_end();
}

enum tags : uint8_t
{
	blue,
	red,
	green
};

void test_plasticity_rule_mask_tagged()
{
	libnux_testcase_begin("plasticity_rule_constant_tagged");

	// generate masks
	__vector uint8_t mask = {green, red,   blue, green, red,   blue, green, red,
	                       blue,  green, red,  blue,  green, red,  blue,  blue};
	MaskTagged<1> m;
	m.addresses[0] = 0;
	m.vectors[0] = mask;
	// initialize plasticity rules
	auto rule_red = PlasticityRules::Tagged::Constant<MaskTagged<1>>(m, red, 7);
	auto rule_blue = PlasticityRules::Tagged::Constant<MaskTagged<1>>(m, blue, 3);
	auto rule_green = PlasticityRules::Tagged::Constant<MaskTagged<1>>(m, green, 1);

	// write weights masked
	rule_red.run();
	rule_blue.run();
	rule_green.run();

	// read and test written weights
	__vector uint8_t read0;
	// clang-format off
	asm volatile(
		"sync\n"
		"fxvinx %[read0], %[dls_weight_base], %[i]\n"
		"sync\n"
		: [read0] "=kv"(read0)
		: [dls_weight_base] "b"(dls_weight_base), [i] "r"(m.addresses[0])
		:);
	// clang-format on
	__vector uint8_t expected = {1, 7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3, 3};
	for (uint32_t i = 0; i < 16; ++i) {
		libnux_test_equal(read0[i], expected[i]);
	}

	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_plasticity_rule_constant();
	test_plasticity_rule_mask_tagged();
	libnux_test_summary();
	libnux_test_shutdown();
}
