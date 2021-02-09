#include "libnux/cstring.h"
#include <libnux/unittest.h>
#include <s2pp.h>

int start()
{
	libnux_test_init();

	libnux_testcase_begin("testing cstring");

	{
		char const ref[] = "0123456789";
		char buf1[sizeof(ref)];
		char buf2[sizeof(ref)];
		memset(buf1, 1, sizeof(buf1));
		memset(buf2, 1, sizeof(buf2));
		for (size_t i = 0; i < sizeof(buf1); i++) {
			libnux_test_equal(buf1[i], buf2[i]);
		}

		{
			memcpy(buf1, ref, sizeof(ref));
			for (size_t i = 0; i < sizeof(ref); i++) {
				libnux_test_equal(ref[i], buf1[i]);
			}
		}
	}

	{
		char buf1[] = "0123456789";
		char const ref[] = "1234456890"; // 567 overwritten by 456
		std::memmove(buf1 + 4, buf1 + 3, 3);
		for (size_t i = 0; i < sizeof(ref); i++) {
			libnux_test_equal(ref[i], buf1[i]);
		}
	}

	libnux_testcase_end();

	libnux_test_summary();
	libnux_test_shutdown();

	return 0;
}
