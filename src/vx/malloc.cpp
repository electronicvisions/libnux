#include <stdint.h>
#include "libnux/vx/malloc.h"

#ifdef MALLOC_DEBUG
#include "libnux/vx/mailbox.h"
#endif

namespace libnux::vx {

char* next_alloc = (char*) &heap_base;

extern "C" void* malloc(size_t size)
{
	char* this_alloc = next_alloc;
	intptr_t dist = (intptr_t) &heap_end - (intptr_t) this_alloc;
	void* p = 0;
#ifdef MALLOC_DEBUG
	libnux_mailbox_write_string(" heap_base = ");
	libnux_mailbox_write_int((int) &heap_base);
	libnux_mailbox_write_string(" this_alloc = ");
	libnux_mailbox_write_int((int) this_alloc);
	libnux_mailbox_write_string(" heap_end = ");
	libnux_mailbox_write_int((int) &heap_base);
	libnux_mailbox_write_string(" dist = ");
	libnux_mailbox_write_int(dist);
	libnux_mailbox_write_string(" stack_ptr = ");
	libnux_mailbox_write_int((int) &p);
	libnux_mailbox_write_string("\n");
#endif
	if (((intptr_t) this_alloc >= (intptr_t) &p) || (dist < 0) || ((uintptr_t)(dist) < size))
		return 0;
	next_alloc += size;
	return this_alloc;
}

extern "C" void free(void* ptr)
{
	// do nothing; ever-growing heap...
	(void) (ptr);
}

} // namespace libnux::vx
