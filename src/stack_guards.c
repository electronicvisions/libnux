#include <stdint.h>

#ifdef LIBNUX_STACK_PROTECTOR

#define STACK_GUARD_MAGIC 0xdeadbeef

extern void exit(uint32_t);

uintptr_t __stack_chk_guard = STACK_GUARD_MAGIC;

void __stack_chk_fail(void)
{
	exit(STACK_GUARD_MAGIC);
}

#endif

#ifdef LIBNUX_STACK_REDZONE

extern uint32_t stack_redzone;

void __attribute__((naked)) isr_program(void)
{
	// clang-format off
	asm volatile(
		"cmplw cr7, %0, %%sp\n"
		// return ENOMEM=12 on stack overflow
		"li %%r3, 12\n"
		"blt cr7, exit\n"
		// return 1 otherwise
		"li %%r3, 1\n"
		"bl exit\n"
		// fstack-limit-symbol=sym limits at sym+48
		:: "r"(stack_redzone + 48) :
	);
	// clang-format on
}

#endif
