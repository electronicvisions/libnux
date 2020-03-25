/*
 * This program runs all the programs generated with the C-compiler
 * */

.extern reset
.extern exit
.extern _start

.extern _isr_undefined
.extern isr_einput
.extern isr_alignment
.extern isr_program
.extern isr_doorbell
.extern isr_fit
.extern isr_dec

.extern stack_ptr_init

.globl __init
.type __init, @function

# Code section
.section .text.crt_int

reset:
	b __init

	# interrupt jump table
	int_mcheck:    b _isr_undefined
	int_cinput:    b _isr_undefined
	int_dstorage:  b _isr_undefined
	int_istorage:  b _isr_undefined
	int_einput:    b isr_einput
	int_alignment: b isr_alignment
	int_program:   b isr_program
	int_syscall:   b _isr_undefined
	int_doorbell:  b isr_doorbell
	int_cdoorbell: b _isr_undefined
	int_fit:       b isr_fit
	int_dec:       b isr_dec

__init:
	# set the stack pointer
	lis 1, stack_ptr_init@h
	addi 1, 1, stack_ptr_init@l
	# construct global things
	bl __call_constructors
	# start actual program
	bl _start
	# destruct global things
	bl __call_destructors
	# call exit
	bl exit
