/*
 * Runtime Shutdown
 *
 *  * Exit code handling
 *    We expect the exit code of a program to be placed in r3 prior to calling exit.
 *  * stop/end_loop
 *    Infinite wait loop
 */

.extern stack_ptr_init

# External callable functions
.globl exit
.type exit, @function

exit:
	# load stack base into r11 and save the current stack pointer to the
	# stack base
	lis 11, stack_ptr_init@h
	addi 11, 11, stack_ptr_init@l
	stw 1, 0(11)
	# push return value from r3 to the SP + 12 to have the lowest stack
	# frame look like:
	# | SP + 0      | SP + 4      | SP + 8      | SP + 12     |
	# +-------------+-------------+-------------+-------------+
	# | aa aa aa aa | bb bb bb bb | 00 00 00 00 | cc cc cc cc |
	# where "a" is the saved stack pointer "b" is the saved link register
	# and "c" the return code
	stw 3, 12(11)

stop:
end_loop:
	wait
	b end_loop

