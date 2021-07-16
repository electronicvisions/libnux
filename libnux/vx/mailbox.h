#pragma once

#include <s2pp.h>
#include <cstdint>

extern uint8_t mailbox_base;
extern uint8_t mailbox_end;

namespace libnux::vx {

uint32_t mailbox_write(uint32_t const offset, uint8_t const * src, uint32_t const size);
uint32_t mailbox_read(uint8_t * dest, uint32_t const offset, uint32_t const size);

uint8_t mailbox_read_u8(uint32_t const offset);
void mailbox_write_u8(uint32_t const offset, uint8_t byte);

/**
 * Null-terminate any string in the mailbox.
 */
void mailbox_string_terminate();

uint32_t mailbox_write_string(char const * str);
uint32_t mailbox_write_int(uint32_t const n);
uint32_t mailbox_write_signed_int(int32_t const n);

uint32_t mailbox_write_vector(__vector uint8_t const& vec);
uint32_t mailbox_write_vector(__vector uint16_t const& vec);
uint32_t mailbox_write_signed_vector(__vector int8_t const& vec);
uint32_t mailbox_write_signed_vector(__vector int16_t const& vec);

} // namespace libnux::vx
