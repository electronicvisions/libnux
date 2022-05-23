#pragma once

#include <cstdint>
#include "libnux/vx/omnibus.h"

namespace libnux::vx {

enum class PPUOnDLS : uint32_t
{
	top = 0,
	bottom = 1
};

#include "libnux/vx/dls.tcc"

} // namespace libnux::vx
