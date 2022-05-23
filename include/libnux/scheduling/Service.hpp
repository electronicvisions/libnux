#pragma once
#include "libnux/scheduling/types.hpp"

// Service is a template that can execute
// an arbitrary function or class member function
// by calling service.exec().
// A Service is identified by ID.

// Service for functions.
template <service_id ID, void (*Function)()>
class Service_Function
{
public:
	static constexpr service_id id = ID;
	Service_Function() {}
	static constexpr auto exec = Function;
};

// Service for class member functions.
template <service_id ID, typename C, void (C::*function)()>
class Service_Class
{
	C& m_c;

public:
	static constexpr service_id id = ID;
	Service_Class() {}
	Service_Class(C& c) : m_c(c) {}
	inline __attribute__((always_inline)) auto exec() -> decltype((this->m_c.*function)())
	{
		return (this->m_c.*function)();
	}
};
