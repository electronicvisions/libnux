#include "libnux/vector_helper.h"

#define SHIFT_VECTOR(type, name, select_name, direction, dls_spec)\
void shift_vector_##name(__vector type &shift_vec, size_t shift)\
{\
\
	asm volatile(\
		"sync"\
	);\
	for(size_t i = 0; i < shift; i++) {\
		asm volatile(\
			"fxvsh"#select_name" %[vec], %[vec], "#direction"\n"\
			: [vec] "="#dls_spec"v" (shift_vec)\
			:\
			:\
		);\
	}\
	asm volatile(\
		"sync"\
	);\
}

#ifndef LIBNUX_DLS_VERSION_VX
SHIFT_VECTOR(uint8_t, left_u8, b, 1, k)
SHIFT_VECTOR(uint8_t, right_u8, b, -1, k)
SHIFT_VECTOR(uint16_t, left_u16, h, 1, k)
SHIFT_VECTOR(uint16_t, right_u16, h, -1, k)
#else
SHIFT_VECTOR(uint8_t, left_u8, b, 1, q)
SHIFT_VECTOR(uint8_t, right_u8, b, -1, q)
SHIFT_VECTOR(uint16_t, left_u16, h, 1, q)
SHIFT_VECTOR(uint16_t, right_u16, h, -1, q)
#endif

#undef SHIFT_VECTOR
