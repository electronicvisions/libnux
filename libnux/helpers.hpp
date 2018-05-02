#pragma once

/*
	Prevent the compiler from optimizing away variables or expressions, even
	if the result is not used after calculation or known at compile time.
*/
template <class T>
__attribute__((always_inline)) inline void do_not_optimize_away(T const& value)
{
	asm volatile("" : "+m"(const_cast<T&>(value)));
}
