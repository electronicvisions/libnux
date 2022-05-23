#include "libnux/vx/globaladdress.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

auto to_hexstring(uint32_t const value)
{
	// creates 0x....'....
	std::array<char, 12> ret;
	char constexpr hex_index[] = "0123456789abcdef";
	ret[ 0] = '0';
	ret[ 1] = 'x';
	ret[ 2] = hex_index[(*((uint8_t*)(&value) + 0) >> 4) & 0xf];
	ret[ 3] = hex_index[(*((uint8_t*)(&value) + 0)     ) & 0xf];
	ret[ 4] = hex_index[(*((uint8_t*)(&value) + 1) >> 4) & 0xf];
	ret[ 5] = hex_index[(*((uint8_t*)(&value) + 1)     ) & 0xf];
	ret[ 6] = '\'';
	ret[ 7] = hex_index[(*((uint8_t*)(&value) + 2) >> 4) & 0xf];
	ret[ 8] = hex_index[(*((uint8_t*)(&value) + 2)     ) & 0xf];
	ret[ 9] = hex_index[(*((uint8_t*)(&value) + 3) >> 4) & 0xf];
	ret[10] = hex_index[(*((uint8_t*)(&value) + 3)     ) & 0xf];
	ret[11] = 0;
	return ret;
}

void start()
{
	test_init();
	uint32_t next_offset = 0x100;

	{
		testcase_begin("Vector (generated) write to extmem (vector address via vector type), scalar read (generated) of one element");
		auto const ga = GlobalAddress::from_global(AddressSpace::extmem.to_global_omnibus() + next_offset);
		next_offset += 0x100;
		*ga.to_extmem().to_scalar<__vector uint16_t>() = vec_splat_u16(0x1);
		auto val = *ga.to_extmem().to_scalar<uint16_t>();
		test_equal(val, 0x1);
		testcase_end();
		test_write_string("at address: ");
		test_write_string(to_hexstring(ga.to_global_omnibus()).data());
		test_write_string("\n");
	}

	{
		testcase_begin("Vector (fxvstax) write to extmem (scalar address), scalar read (generated) of one element");
		auto const ga = GlobalAddress::from_global(AddressSpace::extmem.to_global_omnibus() + next_offset);
		next_offset += 0x100;
		__vector uint8_t data = vec_splat_u8(0x2);
		// clang-format off
		asm volatile(
			"fxvstax %[data], %[base], %[index]\n"
			"sync\n"
			:: [data] "qv" (data), [base] "b" (ga.to_extmem().to_scalar_addr()), [index] "r" (0) : "memory"
		);
		// clang-format on
		auto const val = *ga.to_extmem().to_scalar<uint8_t>();
		test_equal(val, 0x2);
		testcase_end();
		test_write_string("at address: ");
		test_write_string(to_hexstring(ga.to_global_omnibus()).data());
		test_write_string("\n");
	}

	{
		testcase_begin("Vector (fxvoutx) write to extmem (hioff(scalar) >> 4 | 1<<31), scalar read (generated) of one element");
		auto const ga = GlobalAddress::from_global(AddressSpace::extmem.to_global_omnibus() + next_offset);
		next_offset += 0x100;
		__vector uint8_t data = vec_splat_u8(0x9);
		uint32_t const address = ((ga.to_extmem().to_scalar_addr() & 0x3fff'ffff) >> 4) | (1ull<<31);
		// clang-format off
		asm volatile(
			"fxvoutx %[data], %[base], %[index]\n"
			"sync\n"
			:: [data] "qv" (data), [base] "b" (address), [index] "r" (0) : "memory"
		);
		// clang-format on
		auto const val = *ga.to_extmem().to_scalar<uint8_t>();
		test_equal(val, 0x9);
		testcase_end();
		test_write_string("at address: ");
		test_write_string(to_hexstring(ga.to_global_omnibus()).data());
		test_write_string("\n");
	}

	{
		testcase_begin("Vector (fxvoutx) write to extmem (fxviox address), scalar read (generated) of one element");
		auto const ga = GlobalAddress::from_global(AddressSpace::extmem.to_global_omnibus() + next_offset);
		next_offset += 0x100;
		__vector uint8_t data = vec_splat_u8(0xb);
		// clang-format off
		asm volatile(
			"fxvoutx %[data], %[base], %[index]\n"
			"sync\n"
			:: [data] "qv" (data), [base] "b" (ga.to_extmem().to_fxviox_addr()), [index] "r" (0) : "memory"
		);
		// clang-format on
		auto const val = *ga.to_extmem().to_scalar<uint8_t>();
		test_equal(val, 0xb);
		testcase_end();
		test_write_string("at address: ");
		test_write_string(to_hexstring(ga.to_global_omnibus()).data());
		test_write_string("\n");
	}

	test_summary();
	test_shutdown();
}
