#include "libnux/vx/barrier.h"
#include "libnux/vx/location.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/time.h"

libnux::vx::Barrier<1> barrier;

int start()
{
	std::array<libnux::vx::Barrier<1>*, 1> const barrier_locations{&barrier};

	barrier.initialize(barrier_locations, 0);

	for (size_t counter = 0; counter < 1000; ++counter) {
		barrier.wait();
	}

	libnux::vx::times<1> t;
	for (auto& p : t) {
		p.first = libnux::vx::now();
		for (size_t i = 0; i < 1000; ++i) {
			barrier.wait();
		}
		p.second = libnux::vx::now();
	}
	libnux::vx::print_measurement("1000x barrier.wait()", t);
	return 0;
}
