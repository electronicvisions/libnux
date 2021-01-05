#include "libnux/vx/globaladdress.h"
#include "libnux/vx/unittest.h"

using namespace libnux::vx;

void start()
{
	test_init();

	{
		constexpr auto a = GlobalAddress::from_global(0, 3);
		constexpr auto b = GlobalAddress::from_global(0, 2);
		constexpr auto c = GlobalAddress::from_global(1, 1);
		static_assert((a + b) == c);
		static_assert(a + 2 == c);
	}

	{
		// Write to 0x1234 in extmem
		constexpr auto a = GlobalAddress::from_global(1ull << 31 | 0x1234);

		uint32_t volatile* ptr_scalar = a.to_extmem().to_scalar<uint32_t>();
		*(ptr_scalar + 17) = 17 + 4;
		*(ptr_scalar + 18) = 17 + 4;
		test_equal(*ptr_scalar, 17ull + 4);

		__vector uint8_t tmp;
		__vector uint8_t mask;
		tmp = vec_splat_u8(17 + 5);
		mask = vec_splat_u8(0);
		mask[17] = 1;
		auto vector_addr = a.to_extmem().to_vector_addr();
		asm volatile("fxvcmpb %[mask]\n"
		             "fxvstax %[data], %[base], %[index]\n"
		             "sync\n" ::[mask] "qv"(mask),
		             [data] "qv"(tmp), [base] "b"(vector_addr), [index] "r"(0)
		             : "memory");
		test_equal(*(ptr_scalar + 17), 17ull + 5);
		test_equal(*(ptr_scalar + 18), 17ull + 4);
	}

	{
		constexpr auto a = GlobalAddress::from_global(1ull << 31 | 0x1234);
		test_equal(
		    (uintptr_t) a.to_extmem().to_vector<__vector uint16_t>(), (0x8000'0000ull | (0x1234)));
		test_equal(
		    (uintptr_t) a.to_extmem().to_scalar<__vector uint16_t>(),
		    (0x4000'0000ull | (0x1234 << 2)));
		static_assert(a.to_extmem().to_vector_addr() == (0x8000'0000ull | (0x1234)));
		static_assert(a.to_extmem().to_scalar_addr() == (0x4000'0000ull | (0x1234 << 2)));

		constexpr auto b = GlobalAddress::from_relative<GlobalAddress::ExtMem>(0x1234 << 2);
		test_equal(
		    (intptr_t) a.to_extmem().to_scalar<uint32_t>(),
		    (intptr_t) b.to_extmem().to_scalar<uint32_t>());
		static_assert(a == b);
	}

	{
		constexpr auto b = GlobalAddress::from_global(AddressSpace::extmem.to_global_omnibus() - 1);
		constexpr auto s = GlobalAddress::from_global(AddressSpace::extmem);
		constexpr auto e = GlobalAddress::from_global(
		    AddressSpace::extmem.to_global_omnibus() + AddressSpace::extmem_size);
		constexpr auto m = GlobalAddress::from_global(
		    AddressSpace::extmem.to_global_omnibus() + AddressSpace::extmem_size - 1);
		test_equal(static_cast<bool>(b.to_extmem()), false);
		test_equal(static_cast<bool>(s.to_extmem()), true);
		test_equal(static_cast<bool>(e.to_extmem()), false);
		test_equal(static_cast<bool>(m.to_extmem()), true);
		static_assert(static_cast<bool>(b.to_extmem()) == false);
		static_assert(static_cast<bool>(s.to_extmem()) == true);
		static_assert(static_cast<bool>(e.to_extmem()) == false);
		static_assert(static_cast<bool>(m.to_extmem()) == true);
	}

	{
		constexpr auto b =
		    GlobalAddress::from_global(AddressSpace::sram_top.to_global_omnibus() - 1);
		constexpr auto s = GlobalAddress{AddressSpace::sram_top};
		constexpr auto e = GlobalAddress::from_global(
		    AddressSpace::sram_top.to_global_omnibus() + AddressSpace::sram_size);
		constexpr auto m = GlobalAddress::from_global(
		    AddressSpace::sram_top.to_global_omnibus() + AddressSpace::sram_size - 1);
		test_equal(static_cast<bool>(b.to_sram()), false);
		test_equal(static_cast<bool>(s.to_sram()), true);
		test_equal(static_cast<bool>(e.to_sram()), false);
		test_equal(static_cast<bool>(m.to_sram()), true);
		static_assert(static_cast<bool>(b.to_sram()) == false);
		static_assert(static_cast<bool>(s.to_sram()) == true);
		static_assert(static_cast<bool>(e.to_sram()) == false);
		static_assert(static_cast<bool>(m.to_sram()) == true);
	}

	{
		constexpr auto b =
		    GlobalAddress::from_global(AddressSpace::sram_bot.to_global_omnibus() - 1);
		constexpr auto s = GlobalAddress{AddressSpace::sram_bot};
		constexpr auto e = GlobalAddress::from_global(
		    AddressSpace::sram_bot.to_global_omnibus() + AddressSpace::sram_size);
		constexpr auto m = GlobalAddress::from_global(
		    AddressSpace::sram_bot.to_global_omnibus() + AddressSpace::sram_size - 1);
		test_equal(static_cast<bool>(b.to_sram()), false);
		test_equal(static_cast<bool>(s.to_sram()), true);
		test_equal(static_cast<bool>(e.to_sram()), false);
		test_equal(static_cast<bool>(m.to_sram()), true);
		static_assert(static_cast<bool>(b.to_sram()) == false);
		static_assert(static_cast<bool>(s.to_sram()) == true);
		static_assert(static_cast<bool>(e.to_sram()) == false);
		static_assert(static_cast<bool>(m.to_sram()) == true);
	}

	{
		constexpr auto c =
		    GlobalAddress::from_relative<GlobalAddress::SRAM>(0x123, PPUOnDLS::bottom);
		test_equal(
		    (intptr_t) c.to_sram().to_vector<__vector uint8_t>(),
		    (1 << 27 | 1 << 26 | 1 << 25 | 0x123 << 2));
		static_assert(c.to_sram().to_vector_addr() == 0x123);
		test_equal(
		    (intptr_t) c.to_sram().to_scalar<__vector uint8_t>(),
		    (1 << 27 | 1 << 26 | 1 << 25 | 0x123 << 2));
		static_assert(c.to_sram().to_scalar_addr() == 0x123);
		static_assert(c.to_sram().to_scalar_addr(PPUOnDLS::bottom) == 0x123);
		static_assert(
		    c.to_sram().to_scalar_addr(PPUOnDLS::top) ==
		    (1ull << 31 | 1 << 27 | 1 << 26 | 1 << 25 | 0x123));

		test_equal(c.to_sram().is_local(PPUOnDLS::top), false);
		test_equal(c.to_sram().is_local(PPUOnDLS::bottom), true);
		static_assert(c.to_sram().is_local(PPUOnDLS::top) == false);
		static_assert(c.to_sram().is_local(PPUOnDLS::bottom) == true);
	}

	{
		constexpr auto d =
		    GlobalAddress::from_relative<GlobalAddress::VectorGenerator>(0x0, PPUOnDLS::top);
		test_equal((intptr_t) d.to_vecgen().to_scalar<uint32_t>(), (1 << 30 | 1 << 28 | 0 << 14));
		static_assert(d.to_vecgen().to_scalar_addr() == (1 << 30 | 1 << 28 | 0 << 12));
		static_assert(d.to_vecgen().to_vector_addr() == (1 << 30 | 1 << 28 | 0 << 14));
		test_equal(static_cast<bool>(d.to_vecgen()), true);
		static_assert(static_cast<bool>(d.to_vecgen()) == true);
	}

	{
		constexpr auto d =
		    GlobalAddress::from_relative<GlobalAddress::VectorGenerator>(0x0, PPUOnDLS::bottom);
		test_equal((intptr_t) d.to_vecgen().to_scalar<uint32_t>(), (1 << 30 | 1 << 28 | 1 << 14));
		static_assert(d.to_vecgen().to_scalar_addr() == (1 << 30 | 1 << 28 | 1 << 14));
		static_assert(d.to_vecgen().to_vector_addr() == (1 << 30 | 1 << 28 | 1 << 14));
		test_equal(static_cast<bool>(d.to_vecgen()), true);
		static_assert(static_cast<bool>(d.to_vecgen()) == true);
	}

	{
		constexpr auto b =
		    GlobalAddress::from_global(AddressSpace::vecgen_top.to_global_omnibus() - 1);
		constexpr auto s = GlobalAddress{AddressSpace::vecgen_top};
		constexpr auto e = GlobalAddress::from_global(
		    AddressSpace::vecgen_top.to_global_omnibus() + AddressSpace::vecgen_size);
		constexpr auto m = GlobalAddress::from_global(
		    AddressSpace::vecgen_top.to_global_omnibus() + AddressSpace::vecgen_size - 1);
		test_equal(static_cast<bool>(b.to_vecgen()), false);
		test_equal(static_cast<bool>(s.to_vecgen()), true);
		test_equal(static_cast<bool>(e.to_vecgen()), false);
		test_equal(static_cast<bool>(m.to_vecgen()), true);
		static_assert(static_cast<bool>(b.to_vecgen()) == false);
		static_assert(static_cast<bool>(s.to_vecgen()) == true);
		static_assert(static_cast<bool>(e.to_vecgen()) == false);
		static_assert(static_cast<bool>(m.to_vecgen()) == true);
	}

	{
		constexpr auto b =
		    GlobalAddress::from_global(AddressSpace::vecgen_bot.to_global_omnibus() - 1);
		constexpr auto s = GlobalAddress{AddressSpace::vecgen_bot};
		constexpr auto e = GlobalAddress::from_global(
		    AddressSpace::vecgen_bot.to_global_omnibus() + AddressSpace::vecgen_size);
		constexpr auto m = GlobalAddress::from_global(
		    AddressSpace::vecgen_bot.to_global_omnibus() + AddressSpace::vecgen_size - 1);
		test_equal(static_cast<bool>(b.to_vecgen()), false);
		test_equal(static_cast<bool>(s.to_vecgen()), true);
		test_equal(static_cast<bool>(e.to_vecgen()), false);
		test_equal(static_cast<bool>(m.to_vecgen()), true);
		static_assert(static_cast<bool>(b.to_vecgen()) == false);
		static_assert(static_cast<bool>(s.to_vecgen()) == true);
		static_assert(static_cast<bool>(e.to_vecgen()) == false);
		static_assert(static_cast<bool>(m.to_vecgen()) == true);
	}

	test_summary();
	test_shutdown();
}
