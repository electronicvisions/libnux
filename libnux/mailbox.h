#pragma once

#include <s2pp.h>
#include <stdint.h>

extern uint8_t mailbox_base; // from linker
extern uint8_t mailbox_end;  // from linker

/** Set a different mailbox base and end address than provided by linker.
 * @param base Base or start address of mailbox.
 * @param end End address of mailbox.
 */
void set_mailbox(uint8_t* base, uint8_t* end);

uint32_t mailbox_write(uint32_t const offset, uint8_t const * src, uint32_t const size);
uint32_t mailbox_read(uint8_t * dest, uint32_t const offset, uint32_t const size);

uint8_t libnux_mailbox_read_u8(uint32_t const offset);
void libnux_mailbox_write_u8(uint32_t const offset, uint8_t byte);

/**
 * Null-terminate any string in the mailbox.
 */
void libnux_mailbox_string_terminate();

uint32_t libnux_mailbox_write_string(char const * str);
uint32_t libnux_mailbox_write_int(uint32_t const n);
uint32_t libnux_mailbox_write_signed_int(int32_t const n);
uint32_t libnux_mailbox_write_vector_uint8(__vector uint8_t const vec);
uint32_t libnux_mailbox_write_vector_int8(__vector int8_t const vec);
uint32_t libnux_mailbox_write_vector_uint16(__vector uint16_t const vec);
uint32_t libnux_mailbox_write_vector_int16(__vector int16_t const vec);
