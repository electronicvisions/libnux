#pragma once

#include "libnux/vx/dls.h"
#include <stdint.h>

namespace libnux::vx {

//--------------------------------------------------------------------------------
// Types
//--------------------------------------------------------------------------------

typedef union {
	int32_t words[dls_words_per_vector];
	int16_t halfwords[dls_halfwords_per_vector];
	int8_t bytes[dls_bytes_per_vector];
} fxv_array_t;

typedef enum {
	COND_ALWAYS = 0x0,
	COND_GT     = 0x1,
	COND_LT     = 0x2,
	COND_EQ     = 0x3
} fxv_cond_t;

typedef enum {
	REORDER_TYPE_HALFWORD = 1,
	REORDER_TYPE_BYTE     = 0
} fxv_reorder_t;


//--------------------------------------------------------------------------------
// Other operations
//--------------------------------------------------------------------------------

extern void fxv_zero_vrf();

} // namespace libnux::vx
