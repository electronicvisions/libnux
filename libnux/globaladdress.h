#include <cstdint>

#if __has_include(<s2pp.h>)
// ppu include to support __vector keyword
#include <s2pp.h>
#define htonl(x) x;
#define ntohl(x) x;
typedef uintptr_t ppu_uintptr_t;
typedef intptr_t ppu_intptr_t;
#else
// define it empty
#define __vector
// endianness conversions
#include <arpa/inet.h>
// target platform sizes
typedef uint32_t ppu_uintptr_t;
typedef int32_t ppu_intptr_t;
#endif

#include <array>

#include "dls_vx.h" // base addresses

#ifndef LIBNUX_DLS_VERSION_VX
#error GlobalAddress et al. only defined for VX.
#endif


namespace libnux {

/** GlobalAddress represents the global address space of all components.
 * Fortunately, there is one single "master" that can access all components:
 * "the executor" of playback memory programs. Hence, we can use the addressing
 * scheme of "playback memory" as a coherent global address format.
 * For a few components, there are "alternative" access methods (or address
 * formats) that can or must be used. E.g. accessing sram_bot from ppu_top
 * requires an on-chip omnibus access via the processors scalar unit accessing
 * a "special address range", but accessing sram_bot from ppu_bot is a "simple"
 * memory access. In some cases access methods are not available, e.g. the
 * vector unit (VU) of ppu_top cannot natively access sram_bot.
 */
class GlobalAddress
{
public:
	// TODO: maybe a thin wrapper providing `to_byte()`?
	typedef ppu_uintptr_t omnibus_address;
	typedef ppu_uintptr_t byte_address;

	// clang-format off
	/** Lookup table for bit(s) to component lookup.
	 *
	 * [hxfpga]/units/synplify_wrapper/source/rtl/omnibus_top.sv
	 * [31]
	 *   0 → [PPU Omnibus & SRAM]
	 *   1 → [FPGA Omnibus & ExtMem]
	 *
	 * [hicann-dls-private]/hicann-dls/units/hx_top/source/hdl/rtl/omnibus_top.sv
	 * [31 ... ... 25]
	 *   0 ... ...  0 → ignore for now (MADC, AnnCore, etc.)
	 *
	 * [hicann-dls-private]/hicann-dls/units/hx_top/source/hdl/rtl/omnibus_ppu.sv
	 * [31 ... ... 25 24 23 22]
	 *   0 ... ...  1  0  0  0  syn ram top
	 *   0 ... ...  1  0  0  1  cadc top
	 *   0 ... ...  1  0  1     "ppu" top (sram)
	 *   0 ... ...  1  1  0  0  syn ram bot
	 *   0 ... ...  1  1  0  1  cadc bot
	 *   0 ... ...  1  1  1     "ppu" bot (sram)
	 *
	 * [hxfpga]/units/synplify_wrapper/source/rtl/omnibus_top.sv
	 * [31 ... 27 ...]
	 *   1 ...  0 → FPGA "ppumem" stuff (cf. below)
	 *   1 ...  1 → FPGA Omnibus
	 *
	 * [hxfpga]/units/synplify_wrapper/source/rtl/omnibus_ppumem.sv
	 * [31 ... 27 26 ...]
	 *   1 ...  0  0 extmem
	 *   1 ...  0  1 vector generator
	 */
	// clang-format on

	/** Construct GlobalAddress from a "global address".
	 *  Omnibus accesses use quad-word-based addresses. The embedded
	 * processors (PPUs) as well as other memory-mapped access schemes
	 * (e.g. ultradingens) use the traditional byte-addressing scheme, i.e.
	 * the address needs to be shifted left by 2 digits. We state the bits
	 * in quad-word addressing scheme to better match m4 "split" format; we
	 * convert to byte address format later. Advantage: 32 bits + 8 bits are
	 * sufficient for storage and return values.
	 * TODO: we currently use 32 bits for the "4-bytes-in-word" offset.
	 */
	static constexpr GlobalAddress from_global(
	    omnibus_address const address, byte_address const word_offset = 0)
	{
		return GlobalAddress(address + (word_offset / 4), word_offset % 4);
	}

	static constexpr GlobalAddress from_global(GlobalAddress const& other)
	{
		return GlobalAddress{other};
	}

	inline GlobalAddress() = default;

	/** Comparison operator. */
	constexpr bool operator==(GlobalAddress const& other) const
	{
		return (m_addr == other.m_addr) && (m_offset == other.m_offset);
	}

	/** Arithmetic operator+=. */
	constexpr GlobalAddress& operator+=(GlobalAddress const& other)
	{
		m_addr += other.m_addr + ((m_offset + other.m_offset) >> 2);
		m_offset = (m_offset + other.m_offset) % 4;
		return *this;
	}

	/** Arithmetic operator+= on bytes.*/
	constexpr GlobalAddress& operator+=(byte_address const& offset)
	{
		m_addr += (m_offset + offset) >> 2;
		m_offset = (m_offset + offset) % 4;
		return *this;
	}

	/** Arithmetic operator+. */
	constexpr GlobalAddress operator+(GlobalAddress const& other) const
	{
		GlobalAddress ret{other};
		ret.m_addr += m_addr + ((ret.m_offset + m_offset) >> 2);
		ret.m_offset = (ret.m_offset + m_offset) % 4;
		return ret;
	}

	/** Arithmetic operator+ on bytes. */
	constexpr GlobalAddress operator+(byte_address const& offset) const
	{
		GlobalAddress ret{*this};
		ret.m_addr += (ret.m_offset + offset) >> 2;
		ret.m_offset = (ret.m_offset + offset) % 4;
		return ret;
	}

	/** Construct from relative byte offsets into memory regions. */
	template <typename T>
	static constexpr GlobalAddress from_relative(byte_address offset);
	template <typename T>
	static constexpr GlobalAddress from_relative(byte_address offset, PPUOnDLS me);

	/** Convert GlobalAddress to its "value" type. */
	constexpr omnibus_address to_global_omnibus() const
	{
		return m_addr;
	}

	/** Convert GlobalAddress to its "value" type. */
	constexpr uint64_t to_global_omnibus_byte() const
	{
		return (static_cast<uint64_t>(m_addr) << 2) | m_offset;
	}


	/** ExtMem address type representing a "external memory" address. */
	class ExtMem
	{
	public:
		/** Convert to "vector unit" address. */
		constexpr byte_address to_vector_addr() const
		{
			return m_ptr | extmem_data_base;
		}

		/** Convert to "vector fxvoutx" address. */
		constexpr byte_address to_fxviox_addr() const
		{
			// Whatever...
			return (m_ptr >> 4) | dls_extmem_base;
		}

		/** Convert to "vector unit" pointer. */
		template <typename T>
		constexpr T* to_vector() const
		{
			// using `__vector T*` isn't possible, so it has to be provided by the caller
			static_assert(
			    std::is_same_v<__vector int8_t, T> || std::is_same_v<__vector uint8_t, T> ||
			    std::is_same_v<__vector int16_t, T> || std::is_same_v<__vector uint16_t, T> ||
			    std::is_same_v<__vector int8_t const, T> ||
			    std::is_same_v<__vector uint8_t const, T> ||
			    std::is_same_v<__vector int16_t const, T> ||
			    std::is_same_v<__vector uint16_t const, T>);
			return reinterpret_cast<T*>(static_cast<ppu_uintptr_t>(to_vector_addr()));
		}


		/** Convert to "scalar unit" address. */
		constexpr byte_address to_scalar_addr() const
		{
			return m_ptr | extmem_data_base;
		}

		/** Convert to "scalar unit" pointer. */
		template <typename T>
		constexpr T* to_scalar() const
		{
			return reinterpret_cast<T*>(to_scalar_addr());
		}

		/** Check validity of pointer (i.e. address range). */
		inline constexpr operator bool() const;

	private:
		friend GlobalAddress;
		inline constexpr ExtMem(omnibus_address address, byte_address offset);

		byte_address m_ptr;
	};


	/** SRAM address type representing a "SRAM memory" address on each PPU. */
	class SRAM
	{
	public:
		/** Convert to "vector unit" address for local access. */
		constexpr byte_address to_vector_addr() const
		{
			return m_ptr;
		}

		/** Convert to "vector unit" pointer.
		 *
		 * Invalid if data isn't local to this PPU (cf. is_local())!
		 */
		template <typename T>
		constexpr __vector T* to_vector() const
		{
			return reinterpret_cast<__vector T*>(to_vector_addr());
		}

		/** Convert to "scalar unit" address for local access. */
		constexpr byte_address to_scalar_addr() const
		{
			return m_ptr;
		}

		/** Convert to "scalar unit" address for local and non-local access. */
		inline constexpr byte_address to_scalar_addr(PPUOnDLS const ppu) const;

		/** Convert to "scalar unit" pointer. */
		template <typename T>
		constexpr T* to_scalar() const
		{
			return reinterpret_cast<T*>(to_scalar_addr());
		}

		/** Convert to "scalar unit" pointer. */
		template <typename T>
		constexpr T* to_scalar(PPUOnDLS const ppu) const
		{
			return reinterpret_cast<T*>(to_scalar_addr(ppu));
		}

		/** Check locality of pointer w.r.t. to "me". */
		constexpr bool is_local(PPUOnDLS const& me) const
		{
			return m_me == me;
		}

		/** Check validity of pointer (i.e. address range). */
		inline constexpr operator bool() const;

	private:
		friend GlobalAddress;
		inline constexpr SRAM(omnibus_address address, byte_address offset, PPUOnDLS me);

		byte_address m_ptr;
		PPUOnDLS m_me;
	};


	/** VectorGenerator address type representing a "vector generator" address. */
	class VectorGenerator
	{
	public:
		/** Convert to "vector unit" address. */
		inline constexpr byte_address to_vector_addr() const;

		/** Convert to "vector fxvoutx" address. */
		inline constexpr byte_address to_fxviox_addr() const;

		/** Convert to "vector unit" pointer. */
		template <typename T>
		constexpr __vector T* to_vector() const
		{
			return reinterpret_cast<__vector T*>(to_vector_addr());
		}

		/** Convert to "scalar unit" address. */
		inline constexpr byte_address to_scalar_addr() const;

		/** Convert to "scalar unit" pointer. */
		template <typename T>
		constexpr T* to_scalar() const
		{
			return reinterpret_cast<T*>(to_scalar_addr());
		}

		/** Check validity of pointer (i.e. address range). */
		inline constexpr operator bool() const;

	private:
		friend GlobalAddress;
		inline constexpr VectorGenerator(omnibus_address address, byte_address offset);

		byte_address m_ptr;
	};


	/** Convert a GlobalAddress to an ExtMem address. */
	constexpr ExtMem to_extmem() const
	{
		return ExtMem(m_addr, m_offset);
	}
	constexpr explicit operator ExtMem() const
	{
		return to_extmem();
	}

	/** Convert a GlobalAddress to an SRAM address. */
	inline constexpr SRAM to_sram() const;
	constexpr explicit operator SRAM() const
	{
		return to_sram();
	}

	/** Convert a GlobalAddress to an VectorGenerator address. */
	inline constexpr VectorGenerator to_vecgen() const;
	constexpr explicit operator VectorGenerator() const
	{
		return to_vecgen();
	}

	/** Check if global address represents a valid address in extmem. */
	inline constexpr bool is_extmem() const;

	/** Check if global address represents a valid address in SRAMs. */
	inline constexpr bool is_sram() const;

	/** Check if global address represents a valid address in vector generators. */
	inline constexpr bool is_vecgen() const;

	/** Manual serialization to network byte order. */
	std::array<typename std::common_type<omnibus_address, byte_address>::type, 2> serialize() const
	{
		std::array<typename std::common_type<omnibus_address, byte_address>::type, 2> ret{};
		static_assert(sizeof(m_addr) == sizeof(uint32_t));
		ret[0] = htonl(m_addr);
		static_assert(sizeof(m_offset) == sizeof(uint32_t));
		ret[1] = htonl(m_offset);
		return ret;
	}

	/** Manual deserialization from network byte order. */
	void deserialize(
	    std::array<typename std::common_type<omnibus_address, byte_address>::type, 2> const& state)
	{
		static_assert(sizeof(m_addr) == sizeof(uint32_t));
		m_addr = ntohl(state[0]);
		static_assert(sizeof(m_offset) == sizeof(uint32_t));
		m_offset = ntohl(state[1]);
	}

private:
	constexpr GlobalAddress(omnibus_address const& addr, byte_address const& offset) :
	    m_addr(addr), m_offset(offset)
	{}

	omnibus_address m_addr;
	byte_address m_offset;
};


/** AddressSpace provides the base addresses and address space sizes for the
 * components that are addressible by GlobalAddress.
 */
struct AddressSpace
{
	/** PPU SRAM. */
	GlobalAddress static constexpr sram_top =
	    GlobalAddress::from_global(0 << 31 | 1 << 25 | 0 << 24 | 1 << 23);
	GlobalAddress static constexpr sram_bot =
	    GlobalAddress::from_global(0 << 31 | 1 << 25 | 1 << 24 | 1 << 23);
	static constexpr uint32_t sram_size{1 << 12}; // 4kWords (or 16kiB) of SRAM
	static_assert(sram_top.to_global_omnibus() == sram_top_base_address);
	static_assert(sram_bot.to_global_omnibus() == sram_bot_base_address);

	/** Vector generator on FPGA. */
	/* FIXME: There seems to be some confusion about changesets in-flight
	 * currently... using what seemed to have worked at some point in time. */
	GlobalAddress static constexpr vecgen_top =
	    GlobalAddress::from_global(1ull << 31 | 0 << 27 | 1 << 26 | 0 << 12);
	GlobalAddress static constexpr vecgen_bot =
	    GlobalAddress::from_global(1ull << 31 | 0 << 27 | 1 << 26 | 1 << 12);
	constexpr static uint32_t vecgen_size{1}; // FIFO-like, not mmapped.
	static_assert(vecgen_top.to_global_omnibus() == 0x8400'0000);
	static_assert(vecgen_bot.to_global_omnibus() == 0x8400'1000);

	/** External memory (FPGA-connected DRAM). */
	GlobalAddress static constexpr extmem =
	    GlobalAddress::from_global(1ull << 31 | 0 << 27 | 0 << 26);
	constexpr static uint32_t extmem_size{1 << 29}; // 128MWords (or 512MiB) of extmem
	static_assert(extmem.to_global_omnibus() == 0x8000'0000);
};


constexpr GlobalAddress::ExtMem::ExtMem(
    GlobalAddress::omnibus_address const address, GlobalAddress::byte_address const offset) :
    m_ptr((address << 2) | offset)
{}

constexpr GlobalAddress::ExtMem::operator bool() const
{
	return m_ptr < static_cast<byte_address>(AddressSpace::extmem_size << 2);
}

constexpr GlobalAddress::SRAM::operator bool() const
{
	bool const is_top =
	    (m_ptr >= static_cast<byte_address>(AddressSpace::sram_top.to_global_omnibus() << 2)) &&
	    (m_ptr < static_cast<byte_address>(
	                 (AddressSpace::sram_top.to_global_omnibus() + AddressSpace::sram_size) << 2));
	bool const is_bot =
	    (m_ptr >= static_cast<byte_address>(AddressSpace::sram_bot.to_global_omnibus() << 2)) &&
	    (m_ptr < static_cast<byte_address>(
	                 (AddressSpace::sram_bot.to_global_omnibus() + AddressSpace::sram_size) << 2));
	return is_top || is_bot;
}

constexpr GlobalAddress::SRAM::SRAM(
    GlobalAddress::omnibus_address const address,
    GlobalAddress::byte_address const offset,
    PPUOnDLS const me) :
    m_ptr((address << 2) | offset), m_me(me)
{}

constexpr GlobalAddress::byte_address GlobalAddress::SRAM::to_scalar_addr(PPUOnDLS const ppu) const
{
	// non-local SRAM via omnibus (convert to byte address), requires bit 31 to be active.
	return m_ptr | ((ppu == m_me)
	                    ? 0
	                    : (ppu_omnibus_chip | (m_me == PPUOnDLS::top)
	                           ? (AddressSpace::sram_top.to_global_omnibus() << 2)
	                           : (AddressSpace::sram_bot.to_global_omnibus() << 2)));
}

constexpr GlobalAddress::VectorGenerator::VectorGenerator(
    GlobalAddress::omnibus_address const address, GlobalAddress::byte_address const offset) :
    m_ptr((address << 2) | offset)
{}

constexpr GlobalAddress::byte_address GlobalAddress::VectorGenerator::to_vector_addr() const
{
	return m_ptr | extmem_data_base;
}

constexpr GlobalAddress::byte_address GlobalAddress::VectorGenerator::to_fxviox_addr() const
{
	// add the bit for access to vector-external world
	return (m_ptr >> 4) | dls_extmem_base;
}

constexpr GlobalAddress::byte_address GlobalAddress::VectorGenerator::to_scalar_addr() const
{
	return m_ptr | extmem_data_base;
}

constexpr GlobalAddress::VectorGenerator::operator bool() const
{
	bool const is_top =
	    (m_ptr >= static_cast<byte_address>(AddressSpace::vecgen_top.to_global_omnibus() << 2)) &&
	    (m_ptr <
	     static_cast<byte_address>(
	         (AddressSpace::vecgen_top.to_global_omnibus() + AddressSpace::vecgen_size) << 2));
	bool const is_bot =
	    (m_ptr >= static_cast<byte_address>(AddressSpace::vecgen_bot.to_global_omnibus() << 2)) &&
	    (m_ptr <
	     static_cast<byte_address>(
	         (AddressSpace::vecgen_bot.to_global_omnibus() + AddressSpace::vecgen_size) << 2));
	return is_top || is_bot;
}

constexpr GlobalAddress::SRAM GlobalAddress::to_sram() const
{
	return SRAM(
	    m_addr, m_offset,
	    (m_addr < AddressSpace::sram_bot.to_global_omnibus()) ? PPUOnDLS::top : PPUOnDLS::bottom);
}

constexpr GlobalAddress::VectorGenerator GlobalAddress::to_vecgen() const
{
	return VectorGenerator(m_addr, m_offset);
}

template <>
constexpr GlobalAddress GlobalAddress::from_relative<GlobalAddress::ExtMem>(
    GlobalAddress::byte_address offset)
{
	omnibus_address const address = (offset >> 2) | AddressSpace::extmem.to_global_omnibus();
	byte_address const byte_offset = offset % 4;
	return GlobalAddress(address, byte_offset);
}

template <>
constexpr GlobalAddress GlobalAddress::from_relative<GlobalAddress::SRAM>(
    GlobalAddress::byte_address offset, PPUOnDLS me)
{
	omnibus_address const base =
	    (me == PPUOnDLS::top ? AddressSpace::sram_top.to_global_omnibus()
	                         : AddressSpace::sram_bot.to_global_omnibus());
	omnibus_address const address = (offset >> 2) | base;
	byte_address const byte_offset = offset % 4;
	return GlobalAddress(address, byte_offset);
}

template <>
constexpr GlobalAddress GlobalAddress::from_relative<GlobalAddress::VectorGenerator>(
    GlobalAddress::byte_address const offset, PPUOnDLS const ppu)
{
	omnibus_address const base = (ppu == PPUOnDLS::top)
	                                 ? AddressSpace::vecgen_top.to_global_omnibus()
	                                 : AddressSpace::vecgen_bot.to_global_omnibus();
	omnibus_address const address = (offset >> 2) | base;
	byte_address const byte_offset = offset % 4;
	return GlobalAddress(address, byte_offset);
}

constexpr bool GlobalAddress::is_extmem() const
{
	return (m_addr >= AddressSpace::extmem.to_global_omnibus()) &&
	       (m_addr < (AddressSpace::extmem.to_global_omnibus() + AddressSpace::extmem_size));
}

constexpr bool GlobalAddress::is_sram() const
{
	return (m_addr >= AddressSpace::sram_top.to_global_omnibus()) &&
	       (m_addr < (AddressSpace::sram_bot.to_global_omnibus() + AddressSpace::sram_size));
}

constexpr bool GlobalAddress::is_vecgen() const
{
	return (m_addr > AddressSpace::vecgen_top.to_global_omnibus()) &&
	       (m_addr < (AddressSpace::vecgen_bot.to_global_omnibus() + AddressSpace::vecgen_size));
}

} // namespace libnux
