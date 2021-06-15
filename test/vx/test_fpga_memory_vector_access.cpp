#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/vector.h"

using namespace libnux::vx;

void start(void) {
	test_init();

	testcase_begin("external memory write read via vector unit");
	vector_type values;
	for (size_t entry = 0; entry < dls_vector_size; ++entry) {
		values[entry] = entry;
	}

	uint32_t const index = 0;

	set_vector(values, dls_extmem_base, index);

	auto const read = get_vector(dls_extmem_base, index);

	for (size_t column = 0; column < 128; ++column) {
		test_equal(read[column], values[column]);
	}
	testcase_end();

	test_summary();
	test_shutdown();
}
