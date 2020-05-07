#include "libnux/unittest.h"

volatile int global_value = 1;

class NonTriviallyConstructible
{
public:
	int value;

	NonTriviallyConstructible() : value(global_value + 2) {}
};

NonTriviallyConstructible global;

int start()
{
	global_value = 0;
	return (global.value == 3 ? 0 : 1);
}
