#pragma once
#include <array>

#include "hate/math.h"
#include "libnux/vx/omnibus.h"

extern "C" void exit(int);

namespace libnux::vx {

struct BarrierFlag
{
	volatile omnibus_word_t local;
	volatile omnibus_word_t* remote;

	BarrierFlag() : local(0), remote(nullptr) {}
};
// Check that local comes first in memory
static_assert(offsetof(BarrierFlag, local) == 0);


/**
 * Dissemination barrier between multiple agents.
 * Communication is performed via remote memory accesses.
 * @tparam N Number of agents to synchronize for via barrier.
 */
template <size_t N>
struct Barrier
{
	constexpr static size_t num_states_parity = 2;
	constexpr static size_t num_flags_per_parity = hate::math::log2(N);
	constexpr static size_t flag_size_in_words = sizeof(BarrierFlag) / sizeof(omnibus_word_t);

	std::array<std::array<BarrierFlag, num_flags_per_parity>, num_states_parity> flags;

	size_t parity;
	omnibus_word_t sense;

	/**
	 * Construct barrier without information about other barrier's locations.
	 */
	Barrier() : flags(), parity(0), sense(true)
	{
		for (auto& f : flags) {
			f.fill(BarrierFlag());
		}
	}

	/**
	 * Initialize barrier with locations of other barriers and the local index in the collection.
	 * The index decides which remote agents to notify for the dissemination to work.
	 * @param locations Locations of other barriers used for memory accesses.
	 * @param index Index of the local barrier instance in the collection.
	 */
	void initialize(std::array<Barrier*, N> const& locations, size_t const index)
	{
		for (size_t parity = 0; parity < num_states_parity; parity++) {
			for (size_t flag = 0; flag < num_flags_per_parity; flag++) {
				size_t const other = (index + hate::math::pow(2, flag)) % N;
				flags[parity][flag].remote = &(locations[other]->flags[parity][flag].local);
			}
		}
	}

	/**
	 * Block until barrier is released.
	 * Disseminates notification information and waits for expected notifications.
	 */
	void wait()
	{
		for (size_t k = 0; k < num_flags_per_parity; k++) {
			BarrierFlag const& flag = flags[parity][k];
			*(flag.remote) = sense;
			asm volatile("sync");
			while (flag.local != sense) {
			}
		}
		if (parity == 1) {
			sense = !sense;
		}
		parity ^= 1;
	}
};

} // namespace libnux::vx
