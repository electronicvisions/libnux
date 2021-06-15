#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/omnibus.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

// generated in hx_top/**/anncore.sv
vector_row_t get_expectation()
{
	vector_row_t ret;

	for (size_t col = 0; col < dls_num_columns; ++col) {
		if (col % 2 == 0) {
			ret.even_columns[col / 2] = 1 << 7 | ((col % 64) << 1);
		} else {
			ret.odd_columns[col / 2] = 1 << 7 | ((col % 64) << 1);
		}
	}
	return ret;
}

// program entry point
void start(void)
{
	test_init();

	testcase_begin("cadc static pattern");
	auto const expectation = get_expectation();
	constexpr size_t row = 0; // arbitrary in [0,256)
	std::array<bool, 2> is_causal = {false, true};
	for (auto const causal : is_causal) {
		auto const cadc_vector =
		    get_row_via_vector(row, causal ? dls_causal_base : dls_acausal_base);
		// static test pattern is same for both top and bottom tree, therefore only top
		// is tested here
		auto const cadc_omnibus = get_row_via_omnibus(
		    row, causal ? cadc_top_causal_base_address : cadc_top_acausal_base_address);
		test_equal(cadc_vector, cadc_omnibus);
		test_equal(cadc_vector, expectation);
	}
	testcase_end();

	test_summary();
	test_shutdown();
}
