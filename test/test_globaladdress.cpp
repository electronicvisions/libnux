#include "libnux/globaladdress.h"
#include "libnux/unittest.h"

using namespace libnux;

void start()
{
	libnux_test_init();

	{
		constexpr auto a = GlobalAddress::from_global(0, 3);
		constexpr auto b = GlobalAddress::from_global(0, 2);
		constexpr auto c = GlobalAddress::from_global(1, 1);
		static_assert((a + b) == c);
		static_assert(a + 2 == c);
	}

	{
		constexpr auto a = GlobalAddress::from_global(1ull << 31 | 0x1234);
		libnux_test_equal(
		    (uintptr_t) a.to_extmem().to_vector<__vector uint16_t>(), (0x4000'0000 | (0x1234 << 2)));
		libnux_test_equal(
		    (uintptr_t) a.to_extmem().to_scalar<__vector uint16_t>(), (0x4000'0000 | (0x1234 << 2)));
		static_assert(a.to_extmem().to_vector_addr() == (0x4000'0000 | (0x1234 << 2)));
		static_assert(a.to_extmem().to_scalar_addr() == (0x4000'0000 | (0x1234 << 2)));

		constexpr auto b = GlobalAddress::from_relative<GlobalAddress::ExtMem>(0x1234 << 2);
		libnux_test_equal(
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
		libnux_test_equal(static_cast<bool>(b.to_extmem()), false);
		libnux_test_equal(static_cast<bool>(s.to_extmem()), true);
		libnux_test_equal(static_cast<bool>(e.to_extmem()), false);
		libnux_test_equal(static_cast<bool>(m.to_extmem()), true);
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
		libnux_test_equal(static_cast<bool>(b.to_sram()), false);
		libnux_test_equal(static_cast<bool>(s.to_sram()), true);
		libnux_test_equal(static_cast<bool>(e.to_sram()), false);
		libnux_test_equal(static_cast<bool>(m.to_sram()), true);
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
		libnux_test_equal(static_cast<bool>(b.to_sram()), false);
		libnux_test_equal(static_cast<bool>(s.to_sram()), true);
		libnux_test_equal(static_cast<bool>(e.to_sram()), false);
		libnux_test_equal(static_cast<bool>(m.to_sram()), true);
		static_assert(static_cast<bool>(b.to_sram()) == false);
		static_assert(static_cast<bool>(s.to_sram()) == true);
		static_assert(static_cast<bool>(e.to_sram()) == false);
		static_assert(static_cast<bool>(m.to_sram()) == true);
	}

	{
		constexpr auto c =
		    GlobalAddress::from_relative<GlobalAddress::SRAM>(0x123, PPUOnDLS::bottom);
		libnux_test_equal(
		    (intptr_t) c.to_sram().to_vector<__vector uint8_t>(), (1 << 27 | 1 << 26 | 1 << 25 | 0x123));
		static_assert(c.to_sram().to_vector_addr() == (1 << 27 | 1 << 26 | 1 << 25 | 0x123));
		libnux_test_equal(
		    (intptr_t) c.to_sram().to_scalar<__vector uint8_t>(), (1 << 27 | 1 << 26 | 1 << 25 | 0x123));
		static_assert(c.to_sram().to_scalar_addr() == (1 << 27 | 1 << 26 | 1 << 25 | 0x123));
		libnux_test_equal(c.to_sram().is_local(PPUOnDLS::top), false);
		libnux_test_equal(c.to_sram().is_local(PPUOnDLS::bottom), true);
		static_assert(c.to_sram().is_local(PPUOnDLS::top) == false);
		static_assert(c.to_sram().is_local(PPUOnDLS::bottom) == true);
	}

	{
		constexpr auto d =
		    GlobalAddress::from_relative<GlobalAddress::VectorGenerator>(0x0, PPUOnDLS::top);
		libnux_test_equal(
		    (intptr_t) d.to_vecgen().to_scalar<uint32_t>(), (1 << 30 | 1 << 28 | 0 << 14));
		static_assert(d.to_vecgen().to_scalar_addr() == (1 << 30 | 1 << 28 | 0 << 12));
		static_assert(d.to_vecgen().to_vector_addr() == (1 << 30 | 1 << 28 | 0 << 14));
		libnux_test_equal(static_cast<bool>(d.to_vecgen()), true);
		static_assert(static_cast<bool>(d.to_vecgen()) == true);
	}

	{
		constexpr auto d =
		    GlobalAddress::from_relative<GlobalAddress::VectorGenerator>(0x0, PPUOnDLS::bottom);
		libnux_test_equal(
		    (intptr_t) d.to_vecgen().to_scalar<uint32_t>(), (1 << 30 | 1 << 28 | 1 << 14));
		static_assert(d.to_vecgen().to_scalar_addr() == (1 << 30 | 1 << 28 | 1 << 14));
		static_assert(d.to_vecgen().to_vector_addr() == (1 << 30 | 1 << 28 | 1 << 14));
		libnux_test_equal(static_cast<bool>(d.to_vecgen()), true);
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
		libnux_test_equal(static_cast<bool>(b.to_vecgen()), false);
		libnux_test_equal(static_cast<bool>(s.to_vecgen()), true);
		libnux_test_equal(static_cast<bool>(e.to_vecgen()), false);
		libnux_test_equal(static_cast<bool>(m.to_vecgen()), true);
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
		libnux_test_equal(static_cast<bool>(b.to_vecgen()), false);
		libnux_test_equal(static_cast<bool>(s.to_vecgen()), true);
		libnux_test_equal(static_cast<bool>(e.to_vecgen()), false);
		libnux_test_equal(static_cast<bool>(m.to_vecgen()), true);
		static_assert(static_cast<bool>(b.to_vecgen()) == false);
		static_assert(static_cast<bool>(s.to_vecgen()) == true);
		static_assert(static_cast<bool>(e.to_vecgen()) == false);
		static_assert(static_cast<bool>(m.to_vecgen()) == true);
	}

	libnux_test_summary();
	libnux_test_shutdown();
}
