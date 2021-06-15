#include <stdint.h>

namespace libnux::vx {

#ifdef LIBNUX_STACK_PROTECTOR

#define STACK_GUARD_MAGIC 0xdeadbeef

extern "C" void exit(uint32_t);

uintptr_t __stack_chk_guard = STACK_GUARD_MAGIC;

extern "C" void __stack_chk_fail(void)
{
	exit(STACK_GUARD_MAGIC);
}

#endif

#ifdef LIBNUX_STACK_REDZONE

extern uint32_t stack_redzone;

// clang-format off
// "naked" function
asm(".globl isr_program\n"
	"isr_program:\n"
	// fstack-limit-symbol=sym limits at sym+48
	"lis 9, stack_redzone@h\n"
	"addi 9, 9, stack_redzone@l\n"
	"addi 9, 9, 48\n"
	"cmplw cr7, 1, 9\n"
	// return ENOMEM=12 on stack overflow
	"li 3, 12\n"
	"blt cr7, exit\n"
	// return 1 otherwise
	"li 3, 1\n"
	"bl exit\n"
);
// clang-format on

#endif

} // namespace libnux
