#include "libnux/vx/dls.h"
#include "libnux/vx/unittest.h"
#include "libnux/vx/helper.h"
#include <array>
#include <stddef.h>
#include <stdint.h>

using namespace libnux::vx;

void start(void)
{
	test_init();

	testcase_begin("access executor node in omnibus");
	// TODO (Issue #4000): Disabled because not possible when instruction unit fetches in parallel.
	//	uint32_t volatile* ptr = (uint32_t volatile*) (executor_omnibus_mask + 1 * sizeof(uint32_t)
	//+ (1 << 14)); 	*ptr = 17 + 4;
	//	// cf. issue #3739
	//	clobber();
	//	test_equal(21ul, *ptr);
	testcase_end();

	test_summary();
	test_shutdown();
}
