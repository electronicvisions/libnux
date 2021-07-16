#include <cstdint>

extern int32_t start();

extern "C" int32_t _start()
{
	return start();
}
