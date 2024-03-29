#pragma once

#include <cstddef>

extern int heap_base;
extern int heap_end;

extern "C" void* malloc(size_t size);

extern "C" void free(void* ptr);
