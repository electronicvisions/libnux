#include <s2pp.h>

#include "libnux/vx/mailbox.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

#define NUM_VECTOR_REGISTERS 32

// Not enough memory to fit 320 * 128 bytes on PPU
#define NUM_VECTORS_TO_USE 1 * NUM_VECTOR_REGISTERS

// Have this function non-optimized, such that it would not be inlined in
// test_many_vectors. This generates an easier to read assembly.
void __attribute__((optimize("O0"))) test_equal(uint32_t a, uint32_t b)
{
	test_equal(a, b);
}

/*
 * Allocate more vectors than vector registers exist.
 * The compiler is therefore forced to do implicit load/stores between
 * registers and memory at some point.
 * From the generated assembly it looks like currently (2018-05-17) only one
 * vector register is used. Spilling in the sense of relocating vectors into
 * the main memory due to missing hardware resources is therefore not tested.
 */
void test_many_vectors()
{
	testcase_begin(__func__);

	__vector uint8_t vectors[NUM_VECTORS_TO_USE];

	for (uint32_t i = 0; i < NUM_VECTORS_TO_USE; i++) {
		vectors[i] = vec_splat_u8(i);
	}

	for (uint32_t i = 0; i < NUM_VECTORS_TO_USE; i++) {
		for (uint8_t j = 0; j < sizeof(__vector uint8_t); j++) {
			// Make sure all vectors are correct, crop to 8bit
			test_equal(vectors[i][j], i % 256);
		}
	}

	testcase_end();
}

int start()
{
	test_init();
	test_many_vectors();
	test_summary();
	test_shutdown();

	return 0;
}
