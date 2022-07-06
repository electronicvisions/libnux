/* This header file contains additional code related to python bindings */

#include "libnux/vx/genpybind.h"

GENPYBIND_TAG_LIBNUX_VX
GENPYBIND_MANUAL({ parent.attr("__variant__") = "pybind11"; })

#include "libnux/vx/frac.h"
