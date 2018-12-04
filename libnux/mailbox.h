#pragma once

#include <s2pp.h>
#include <stdint.h>

extern uint8_t mailbox_base;
extern uint8_t mailbox_end;

uint32_t mailbox_write(uint32_t const offset, uint8_t const * src, uint32_t const size);
uint32_t mailbox_read(uint8_t * dest, uint32_t const offset, uint32_t const size);

uint8_t libnux_mailbox_read_u8(uint32_t const offset);
void libnux_mailbox_write_u8(uint32_t const offset, uint8_t byte);

uint32_t libnux_mailbox_write_string(char const * str);
uint32_t libnux_mailbox_write_int(uint32_t const n);
uint32_t libnux_mailbox_write_signed_int(int32_t const n);
uint32_t libnux_mailbox_write_vector_uint8(vector uint8_t const vec);
uint32_t libnux_mailbox_write_vector_int8(vector int8_t const vec);
uint32_t libnux_mailbox_write_vector_uint16(vector uint16_t const vec);
uint32_t libnux_mailbox_write_vector_int16(vector int16_t const vec);
