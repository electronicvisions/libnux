#pragma once
#include <limits.h>
#include <stdint.h>


uint32_t xorshift32(uint32_t* seed);
uint32_t draw_poisson(uint32_t* seed, uint32_t cutoff, uint32_t dt);
