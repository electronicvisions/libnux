#pragma once

#include "libnux/vx/mailbox.h"

namespace libnux::vx::v1 {

using libnux::vx::mailbox_write;
using libnux::vx::mailbox_read;

using libnux::vx::mailbox_read_u8;
using libnux::vx::mailbox_write_u8;

using libnux::vx::mailbox_string_terminate;

using libnux::vx::mailbox_write_string;
using libnux::vx::mailbox_write_int;

using libnux::vx::mailbox_write_vector;
using libnux::vx::mailbox_write_signed_vector;

} // namespace libnux::vx::v1
