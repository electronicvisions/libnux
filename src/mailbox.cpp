#include <stddef.h>
#include "libnux/mailbox.h"

static uint32_t write_head_offset = 0;
static uint8_t* mailbox_base_real = &mailbox_base;
static uint8_t* mailbox_end_real = &mailbox_end;

void set_mailbox(uint8_t* base, uint8_t* end)
{
	mailbox_base_real = base;
	mailbox_end_real = end;
}

char* itoa(uint32_t value, uint32_t const base) {
	/* Reserve buffer for 32 digits + null byte */
	static char buffer[33] = {0};
	char* ret = buffer + 32;

	/* Check for valid base */
	if((base > 16) || (base < 2)) {
		/* TODO set errno EDOM as soon as implemented */
		return ret;
	}

	/* Number system conversion */
	do {
		ret--;
		*ret = "0123456789abcdef"[value % base];
		value /= base;
	} while (value != 0);

	return ret;
}

uint32_t mailbox_write(uint32_t const offset, uint8_t const * src, uint32_t const size) {
	volatile uint8_t* ptr = mailbox_base_real + offset;
	uint32_t i;

	for(i=size; (ptr < mailbox_end_real) && (i > 0); ptr++, i--) {
		*ptr = *(src++);
	}

	return ptr - mailbox_base_real + offset;
}

uint32_t mailbox_read(uint8_t * dest, uint32_t const offset, uint32_t const size) {
	volatile uint8_t* ptr = mailbox_base_real + offset;
	uint32_t i;

	for(i=size; (dest < mailbox_end_real) && (i > 0); ptr++, i--) {
		*(dest++) = *(ptr);
	}

	return ptr - mailbox_base_real + offset;
}

uint8_t libnux_mailbox_read_u8(uint32_t const offset) {
	if (reinterpret_cast<unsigned long>(mailbox_base_real + offset) <
	    reinterpret_cast<unsigned long>(mailbox_end_real)) {
		return *(mailbox_base_real + offset);
	} else {
		/* TODO set errno ERANGE as soon as implemented */
		return 0;
	}
}

void libnux_mailbox_write_u8(uint32_t const offset, uint8_t const byte) {
	if (reinterpret_cast<unsigned long>(mailbox_base_real + offset) <
	    reinterpret_cast<unsigned long>(mailbox_end_real)) {
		*(mailbox_base_real + offset) = byte;
	} else {
		/* TODO set errno ERANGE as soon as implemented */
	}
}

void libnux_mailbox_string_terminate()
{
	libnux_mailbox_write_u8(write_head_offset++, 0);
}

uint32_t libnux_mailbox_write_string(char const * str) {
	char const * ptr = str;
	for (; (*ptr != 0); write_head_offset++, ptr++) {
		if (mailbox_base_real + write_head_offset == mailbox_end_real) {
			/* TODO set errno ERANGE as soon as implemented */
			return ptr - str;
		}
		*(mailbox_base_real + write_head_offset) = *ptr;
	}
	return ptr - str;
}

uint32_t libnux_mailbox_write_int(uint32_t const n) {
	return libnux_mailbox_write_string(itoa(n, 10));
}

uint32_t libnux_mailbox_write_signed_int(int32_t const n) {
	if (n < 0) {
		uint32_t ret = libnux_mailbox_write_string("-");
		if (n == INT32_MIN) {
			return ret + libnux_mailbox_write_string("2147483648");
		} else {
			return ret + libnux_mailbox_write_int(-n);
		}
	}
	return libnux_mailbox_write_int(n);
}

#define LIBNUX_WRITE_VECTOR(type, name, number_writer)                                             \
	uint32_t libnux_mailbox_write_vector_##name(__vector type const vec)                           \
	{                                                                                              \
		uint32_t ret = 0;                                                                          \
		ret += libnux_mailbox_write_string("{");                                                   \
		for (size_t i = 0; i < sizeof(__vector type) / sizeof(type); ++i) {                        \
			ret += number_writer(vec[i]);                                                          \
			if (i != sizeof(__vector type) / sizeof(type) - 1) {                                   \
				ret += libnux_mailbox_write_string(",");                                           \
			}                                                                                      \
		}                                                                                          \
		return ret + libnux_mailbox_write_string("}");                                             \
	}

LIBNUX_WRITE_VECTOR(uint8_t, uint8, libnux_mailbox_write_int)
LIBNUX_WRITE_VECTOR(int8_t, int8, libnux_mailbox_write_signed_int)
LIBNUX_WRITE_VECTOR(uint16_t, uint16, libnux_mailbox_write_int)
LIBNUX_WRITE_VECTOR(int16_t, int16, libnux_mailbox_write_signed_int)
#undef LIBNUX_WRITE_VECTOR
