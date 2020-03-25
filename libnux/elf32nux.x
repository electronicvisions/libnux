MEMORY {
	int_mem(rwx) : ORIGIN = 0x00000000, LENGTH = 16K
	ext_mem(rwx) : ORIGIN = 0x80000000, LENGTH = 128K
}

/* ECM(2017-12-04): We should provide this numbers from waf configure step FIXME */
ram_base = ORIGIN(int_mem);
ram_end = ORIGIN(int_mem) + LENGTH(int_mem);

/* Default mailbox_size=0
 * May be overwritten by linker using e.g. --defsym=mailbox_size=4096
 */
PROVIDE(mailbox_size = ram_base);

mailbox_end = ram_end;
mailbox_base = mailbox_end - mailbox_size;

ENTRY(__init);

/* In the PowerPC EABI calling convention the link register is saved in the
calling function's stack frame at SP+4. Therefore, the base pointer needs to be
shifted by at least 8 bytes from the mailbox to capture the saved link
register. For debugging purposes -- in particular easier examination of the
stack frames -- the stack base is shifted by 16 bytes for an improved
alignment. */
stack_ptr_init = mailbox_base - 16;

SECTIONS {
	/* . = 0x0; location counter starts implicitly at 0 */
	int.text : {
		_isr_undefined = .;

		/* put internal part of the C runtime first */
		*(.text.crt_int*);

		/* We allow manually placed code via gcc attribute:
		   `__attribute__ ((section ("int.text")));`
		   (cf. attrib.h)
		   To be used, e.g., at function declaration. */
		*(int.text*);

		/* map all undefined isr_* to isr_undefined */
		PROVIDE(isr_einput = _isr_undefined);
		PROVIDE(isr_alignment = _isr_undefined);
		PROVIDE(isr_program = _isr_undefined);
		PROVIDE(isr_doorbell = _isr_undefined);
		PROVIDE(isr_fit = _isr_undefined);
		PROVIDE(isr_dec = _isr_undefined);
	} > int_mem

	/* All data is placed in SRAM */
	int.data : {
		*(.data*);
		*(.rodata*);
	} > int_mem

	int.bss : {
		*(.bss*);
		*(.sbss*);
	} > int_mem

	_int_end = .;
	. = ORIGIN(ext_mem);

	ext.text : {
		/* C-to-C++ static ctors / jump in function next / static dtors /
                   cpp pure virtual handler */
		*(.text.__call_constructors);
		*(.text._start);
		*(.text.__call_destructors);
		KEEP(*(.text.__cxa_pure_virtual*));

		/* user code */
		*(.text*);

		/* explicitly placed user code */
		*(ext.text*);
	} > ext_mem

	ext.init_array : {
		PROVIDE_HIDDEN (__init_array_start = .);
		*(.init_array*);
		KEEP(*(SORT_BY_INIT_PRIORITY(.ctors*)));
		PROVIDE_HIDDEN (__init_array_end = .);
	} > ext_mem

	ext.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		*(.fini_array*);
		KEEP(*(SORT_BY_INIT_PRIORITY(.dtors*)));
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > ext_mem

	/* drop some stuff */
	/DISCARD/ : {
		*(.eh_frame) /* exception handling tables */
		*(.comment) /* whatever... */
		*(.debug_*) /* live fast die young */
	}

	/* global symbol marking the end */
	_ext_end = .;
}
. = _int_end;
. = ALIGN(16);

/*
    With -fstack-limit-symbol=stack_redzone, function entries get checked, if
    stack pointer is below stack_redzone + 48 bytes.
    This offset maybe subject to change with a different compiler version.
*/
stack_redzone = .;

ASSERT( . < mailbox_base, "Filled mailbox ram region, aborting.");

heap_base = .;
heap_end = mailbox_base - 8;
