#include "libnux/barrier.h"
#include "libnux/location.h"
#include "libnux/mailbox.h"
#include "libnux/time.h"

libnux::Barrier<1> barrier;

int start()
{
	std::array<libnux::Barrier<1>*, 1> const barrier_locations{&barrier};

	barrier.initialize(barrier_locations, 0);

	for (size_t counter = 0; counter < 1000; ++counter) {
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
