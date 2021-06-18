#pragma once
#include <limits.h>
#include <s2pp.h>
#include <stdint.h>
#include <cstddef>

uint32_t xorshift32(uint32_t* seed);
uint32_t draw_poisson(uint32_t* seed, uint32_t cutoff, uint32_t dt);
void xorshift_vector(const __vector uint8_t* seed);
uint32_t random_lcg(uint32_t *seed);

void init_random_generator(size_t row, uint8_t seed);
void init_random_generator(size_t row, __vector uint8_t seed_vector);
void make_rand_calls(size_t row, size_t num);
__vector uint8_t get_rand_u8(size_t row);
__vector int8_t get_rand_s8(size_t row);
__vector uint16_t get_rand_u16(size_t row);
__vector uint8_t get_rand_bitwise_u8(size_t row, size_t bit);
__vector uint16_t get_rand_bitwise_u16(size_t row, size_t bit);
