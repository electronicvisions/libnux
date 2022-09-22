MEMORY {
	int_mem(rwx) : ORIGIN = 0x00000000, LENGTH = 16K
	ext_mem(rwx) : ORIGIN = 0x80000000, LENGTH = 128K
	ext_mem_data(rw) : ORIGIN = 0x40000000, LENGTH = 128K /* Same memory as ext_mem */
}

/* ECM(2017-12-04): We should provide this numbers from waf configure step FIXME */
ram_base = ORIGIN(int_mem);
ram_end = ORIGIN(int_mem) + LENGTH(int_mem);

/* Default mailbox_size=0
 * May be overwritten by linker using e.g. --defsym=mailbox_size=4096
 */
PROVIDE(mailbox_size = 0);

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

	/* All data is placed in SRAM by default */
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

	/* drop some stuff */
	/DISCARD/ : {
		*(.eh_frame) /* exception handling tables */
		*(.comment) /* whatever... */
		*(.debug_*) /* live fast die young */
	}

	/* global symbol marking the end of ext_mem text sections */
	_ext_end = .;

	/* all extmem data is placed after text sections */
	ext_data_offset = . - ORIGIN(ext_mem);
	. = ORIGIN(ext_mem_data) + ext_data_offset;
	_ext_data_begin = .;

	ext.init_array (ORIGIN(ext_mem_data) + ext_data_offset) : {
		PROVIDE_HIDDEN (__init_array_start = .);
		*(.init_array*);
		KEEP(*(SORT_BY_INIT_PRIORITY(.ctors*)));
		PROVIDE_HIDDEN (__init_array_end = .);
	} > ext_mem_data
	ext_data_offset = ext_data_offset + SIZEOF(ext.init_array);

	ext.fini_array (ORIGIN(ext_mem_data) + ext_data_offset) :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		*(.fini_array*);
		KEEP(*(SORT_BY_INIT_PRIORITY(.dtors*)));
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > ext_mem_data
	ext_data_offset = ext_data_offset + SIZEOF(ext.fini_array);

	ext.data (ORIGIN(ext_mem_data) + ext_data_offset) :
	{
		*(ext.data*);
		KEEP(*(ext.data.keep*));
	} > ext_mem_data
	ext_data_offset = ext_data_offset + SIZEOF(ext.data);

	/* global symbol marking the end of ext_mem_data data sections */
	_ext_end_data = .;
}
. = _int_end;
. = ALIGN(16);

/*
    With -fstack-limit-symbol=stack_redzone, function entries get checked, if
    stack pointer is below stack_redzone + 32 bytes.
    This offset may be subject to change with a different compiler version.
    We add another 128 bytes on top of the last section as margin for error.
*/
. += 128;
stack_redzone = .;

ASSERT( . < mailbox_base, "Filled mailbox ram region, aborting.");

heap_base = .;
heap_end = mailbox_base - 8;
