#include "libnux/vx/barrier.h"
#include "libnux/vx/location.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/time.h"

constexpr static size_t num_ppus = 2;

libnux::vx::Barrier<num_ppus> barrier;
uint32_t counter;
uint32_t volatile begin = 0;
uint32_t volatile wait = 0;
uint32_t local_index = 0;

using namespace libnux::vx;

int start()
{
	while (begin == 0) {
	}

	std::array<Barrier<num_ppus>*, num_ppus> const barrier_locations{
	    reinterpret_cast<Barrier<num_ppus>*>(
	        reinterpret_cast<uint32_t>(reinterpret_cast<uint8_t volatile*>(&barrier)) +
	        reinterpret_cast<uint32_t>(
	            reinterpret_cast<uint8_t volatile*>(get_omnibus_pointer(0x02800000)))),
	    reinterpret_cast<Barrier<num_ppus>*>(
	        reinterpret_cast<uint32_t>(reinterpret_cast<uint8_t volatile*>(&barrier)) +
	        reinterpret_cast<uint32_t>(
	            reinterpret_cast<uint8_t volatile*>(get_omnibus_pointer(0x03800000)))),
	};

	barrier.initialize(barrier_locations, local_index);

	counter = 0;
	for (; counter < 1000; ++counter) {
		sleep_cycles(wait);
		barrier.wait();
	}

	times<1> t;
	for (auto& p : t) {
		p.first = now();
		for (size_t i = 0; i < 1000; ++i) {
			barrier.wait();
		}
		p.second = now();
	}
	print_measurement("1000x barrier.wait()", t);
	return 0;
}
