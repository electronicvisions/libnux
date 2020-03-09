#include "libnux/dls.h"
#include "libnux/unittest.h"
#include "libnux/omnibus.h"
#include "libnux/syn.h"
#include "libnux/sync.h"

/** Read first word from weight synram region. */
uint32_t get_weights_vector_unit()
{
	__vector uint8_t left;
	__vector uint8_t right;
	get_weights(&left, &right, 0);
	sync();
	return *((uint32_t*) &left);
}

/** Write first word from weight synram region. */
void set_weights_vector_unit(uint32_t const value)
{
	__vector uint8_t left = vec_splat_u8(0);
	__vector uint8_t right = vec_splat_u8(0);
	uint32_t* ptr = (uint32_t*) (&left);
	*ptr = value;
	sync();
	set_weights(&left, &right, 0);
	sync();
}

void test_omnibus()
{
	libnux_testcase_begin("test_omnibus");
	auto const omnibus_address = dls_synapse_array_base;

	set_weights_vector_unit(0x20202020);
	auto read_1 = libnux::omnibus_read(omnibus_address);
	libnux_test_equal(read_1, 0x20202020);

	libnux::omnibus_write(omnibus_address, 0x20);
	auto read_2 = libnux::omnibus_read(omnibus_address);
	libnux_test_equal(read_2, 0x20);
	libnux_test_equal(get_weights_vector_unit(), 0x20);

	libnux_testcase_end();
}

void start()
{
	libnux_test_init();
	test_omnibus();
	libnux_test_shutdown();
}
