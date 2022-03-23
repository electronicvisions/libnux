#include <cstdint>
#include "libnux/vx/helper.h"

using namespace libnux::vx;

extern uint8_t stack_redzone;

/*
    The stack grows infinitely. With '-fstack-limit-symbol=stack_redzone',
    the program exits with error code 12 (= ENOMEM), whereas without this option,
    it wouldn't finish, since the program memory region would get partially overwritten, which
    is however detected beforehand and a dedicated return value 2 is generated.
*/
int32_t start(void)
{
	uintptr_t stack_ptr;
	// return 2 if stack redzone does not work, which is defined here as the stack pointer is
	// smaller equal than the stack_redzone symbol location.
	// clang-format off
	asm volatile (
		// load stack pointer
		"mr %[stack_ptr], 1\n"
		: [stack_ptr] "=r" (stack_ptr) ::
	);
	// clang-format on
	if (stack_ptr <= uintptr_t(&stack_redzone)) {
		return 2;
	}
	int x = 5;
	do_not_optimize_away(x);
	return start();
}
