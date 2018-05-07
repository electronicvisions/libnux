#pragma once
#include "libnux/attrib.h"

// iterator for container class providing only operator[]
template <class C>
struct Iterator
{
	C& c;
	size_t p;

	Iterator(C& c) : c(c), p(0) {}
	Iterator(C& c, size_t p) : c(c), p(p) {}
	bool operator!=(const Iterator& other);
	Iterator& operator++();
	Iterator operator++(int);
	auto operator*() -> decltype(c.operator[](p));
};

template <class C>
ATTRIB_ALWAYS_INLINE bool Iterator<C>::operator!=(const Iterator<C>& other)
{
	return &c != &other.c || p != other.p;
}
template <class C>
ATTRIB_ALWAYS_INLINE Iterator<C>& Iterator<C>::operator++()
{
	++p;
	return *this;
}
template <class C>
ATTRIB_ALWAYS_INLINE Iterator<C> Iterator<C>::operator++(int)
{
	return Iterator(c, p++);
}
template <class C>
ATTRIB_ALWAYS_INLINE auto Iterator<C>::operator*() -> decltype(c.operator[](p))
{
	return c.operator[](p);
}
