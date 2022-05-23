#pragma once

#define ATTRIB_UNUSED __attribute__((unused))
#define ATTRIB_ALIGN_WORD __attribute__((aligned(4)))
#define ATTRIB_ALWAYS_INLINE inline __attribute__((always_inline))
#define ATTRIB_NOINLINE __attribute__((noinline))
