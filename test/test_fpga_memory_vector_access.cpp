#include <array>
#include <stddef.h>
#include <stdint.h>
#include "libnux/dls.h"
#include "libnux/unittest.h"
#include "libnux/vector.h"

using namespace libnux;

void start(void) {
	libnux_test_init();

	libnux_testcase_begin("external memory write read via vector unit");
	vector_type values;
	for (size_t entry = 0; entry < dls_vector_size; ++entry) {
		values[entry] = entry;
	}

	uint32_t const index = 0;

	set_vector(values, dls_extmem_base, index);

	auto const read = get_vector(dls_extmem_base, index);

	for (size_t column = 0; column < 128; ++column) {
		libnux_test_equal(read[column], values[column]);
	}
	libnux_testcase_end();

	libnux_test_summary();
	libnux_test_shutdown();
}
