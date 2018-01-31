MEMORY {
	ram(rwx) : ORIGIN = 0, LENGTH = 16K
}


mailbox_size = 4096;
mailbox_end = 0x4000;
mailbox_base = mailbox_end - mailbox_size;
/* In the PowerPC EABI calling convention the link register is saved in the
calling function's stack frame at SP+4. Therefore, the base pointer needs to be
shifted by at least 8 bytes from the mailbox to capture the saved link
register. For debugging purposes -- in particular easier examination of the
stack frames -- the stack base is shifted by 16 bytes for an improved
alignment. */
stack_ptr_init = mailbox_base - 16;

SECTIONS {
	.text : {
        _isr_undefined = .;

        *crt.s.[0-9].o(.text)
        *(.text)

        PROVIDE(isr_einput = _isr_undefined);
        PROVIDE(isr_alignment = _isr_undefined);
        PROVIDE(isr_program = _isr_undefined);
        PROVIDE(isr_doorbell = _isr_undefined);
        PROVIDE(isr_fit = _isr_undefined);
        PROVIDE(isr_dec = _isr_undefined);
	} > ram

	.data : {
		*(.data)
		*(.rodata)
	} > ram

	.bss : {
		*(.bss)
		*(.sbss)
	} > ram

  /DISCARD/ : {
    *(.eh_frame)
  }
}


