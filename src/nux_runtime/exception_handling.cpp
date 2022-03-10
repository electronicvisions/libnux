#include <cstdlib>

#if !__has_builtin(__builtin_abort)
#define __builtin_abort() do { \
    exit(1); \
} while (0)
#endif


namespace std {
	void __throw_out_of_range_fmt(const char*, ...) {
		__builtin_abort();
	}
} // std

