#pragma once

#include <s2pp.h>
#include <stdint.h>

// Reverse all bits in the given byte
uint8_t reverse_byte(uint8_t b);

// Reverse all bits for all contained bytes in the vector
void vector_reverse_bytes(__vector uint8_t* data);
