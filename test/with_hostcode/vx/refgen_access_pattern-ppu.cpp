#include <array>
#include "libnux/vx/omnibus.h"
#include "libnux/vx/dls.h"

using namespace libnux::vx;

// Reference generator configuration values used within this test
static constexpr std::array<unsigned int, 4> amp_values{5, 10, 15, 20};
static constexpr std::array<unsigned int, 4> slope_values{25, 30, 35, 40};
static constexpr std::array<unsigned int, 4> offset_values{45, 50, 55, 60};


int start()
{
	// Zero-initialize all reference generator config registers
	for (unsigned int i = 21; i < 27; ++i) {
		omnibus_write(madc_base_address + i, 0);
	}

	// refgen_amp configuration
	omnibus_write(madc_base_address + 21, (amp_values.at(1) << 6 | amp_values.at(0)));
	omnibus_write(madc_base_address + 22, (amp_values.at(3) << 6 | amp_values.at(2)));

	// refgen_slope configuration
	omnibus_write(madc_base_address + 23, (slope_values.at(1) << 6 | slope_values.at(0)));
	omnibus_write(madc_base_address + 24, (slope_values.at(3) << 6 | slope_values.at(2)));

	// refgen_offset configuration
	omnibus_write(madc_base_address + 25, (offset_values.at(1) << 6 | offset_values.at(0)));
	omnibus_write(madc_base_address + 26, (offset_values.at(3) << 6 | offset_values.at(2)));

	return 0;
}
