#pragma once

#include <s2pp.h>
#include <cstddef>

void shift_vector_left_u8(__vector uint8_t &shift_vec, size_t shift);
void shift_vector_right_u8(__vector uint8_t &shift_vec, size_t shift);
void shift_vector_left_u16(__vector uint16_t &shift_vec, size_t shift);
void shift_vector_right_u16(__vector uint16_t &shift_vec, size_t shift);
