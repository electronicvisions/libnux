#pragma once

#include "libnux/vx/malloc.h"

namespace std {
typedef size_t size_t;
}

inline void* operator new(std::size_t n) noexcept
{
	return malloc(n);
}

inline void* operator new[](std::size_t n) noexcept
{
	return malloc(n);
}

inline void operator delete(void* p) noexcept
{
	free(p);
}

inline void operator delete[](void* p) noexcept
{
	free(p);
}

inline void operator delete(void* p, std::size_t) noexcept
{
	free(p);
}

inline void operator delete[](void* p, std::size_t) noexcept
{
	free(p);
}

inline void* operator new(std::size_t, void* ptr) noexcept
{
	return ptr;
}

inline void* operator new[](std::size_t, void* ptr) noexcept
{
	return ptr;
}
