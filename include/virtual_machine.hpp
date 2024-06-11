#pragma once

#include <cstdint>
#include <ostream>

namespace plzerow {
enum OP_CODE : std::uint8_t { OP_RETURN, OP_CONSTANT, OP_CONSTANT_LONG };

std::ostream &operator<<(std::ostream &os, OP_CODE color);

} // namespace plzerow
