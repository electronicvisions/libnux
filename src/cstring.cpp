#include "libnux/cstring.h"

// Minimal implementation of some typical functions declared in C's string.h

void* memset(void* dst, int ch, size_t len)
{
	auto pDst = static_cast<unsigned char*>(dst);

	while (len--) {
		*(pDst++) = static_cast<unsigned char>(ch);
	}

	return dst;
}

void* memcpy(void* dst, void const* src, size_t len)
{
	auto pDst = static_cast<char*>(dst);
	auto pSrc = static_cast<char const*>(src);

	while (len--) {
		*pDst++ = *pSrc++;
	}

	return dst;
}

void* memmove(void* dst, void const* src, size_t len)
{
	auto pDst = static_cast<char*>(dst);
	auto pSrc = static_cast<char const*>(src);

	if (pSrc < pDst) {
		pDst += len;
		pSrc += len;
		while (len--) {
			*--pDst = *--pSrc;
		}
	} else {
		while (len--) {
			*pDst++ = *pSrc++;
		}
	}

	return dst;
}
