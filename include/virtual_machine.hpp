#pragma once

#include <cstdint>
#include <ostream>

namespace gbpl0 {
enum OP_CODE : std::uint8_t { OP_RETURN, OP_CONSTANT };

std::ostream &operator<<(std::ostream &os, OP_CODE color);

} // namespace gbpl0
