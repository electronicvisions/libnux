#include "libnux/vx/dls.h"
#include "libnux/vx/location.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/omnibus.h"
#include "libnux/vx/syn.h"
#include "libnux/vx/sync.h"

using namespace libnux::vx;

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
	testcase_begin("test_omnibus");

	PPUOnDLS ppu;
	bool const success = get_location(ppu);
	if (!success) {
		exit(1);
	}

	omnibus_address_t omnibus_address =
	    (ppu == PPUOnDLS::top) ? synram_top_base_address : synram_bottom_base_address;

	set_weights_vector_unit(0x20202020);
	auto read_1 = omnibus_read(omnibus_address);
	test_equal(read_1, static_cast<uint32_t>(0x20202020));

	omnibus_write(omnibus_address, 0x20);
	auto read_2 = omnibus_read(omnibus_address);
	test_equal(read_2, static_cast<uint32_t>(0x20));
	test_equal(get_weights_vector_unit(), static_cast<uint32_t>(0x20));

	testcase_end();
}

void start()
{
	test_init();
	test_omnibus();
	test_shutdown();
}
