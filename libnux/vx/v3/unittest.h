#pragma once

#include "libnux/vx/unittest.h"

namespace libnux::vx::v3 {

using libnux::vx::test_write_string;
using libnux::vx::test_write_int;
using libnux::vx::test_write_termination;

using libnux::vx::test_init;
using libnux::vx::test_shutdown;

using libnux::vx::test_action_type;

using libnux::vx::test_get_passed;
using libnux::vx::test_get_failed;

using libnux::vx::testcase_get_passed;
using libnux::vx::testcase_get_failed;

using libnux::vx::test_set_action;
using libnux::vx::test_get_action;

using libnux::vx::test_inc_passed;
using libnux::vx::test_inc_failed;

using libnux::vx::test_equal;
using libnux::vx::test_true;
using libnux::vx::test_null;
using libnux::vx::test_not_null;

using libnux::vx::testcase_begin;
using libnux::vx::testcase_end;
using libnux::vx::test_summary;

} // namespace libnux::vx::v3
