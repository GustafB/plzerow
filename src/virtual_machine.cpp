#include "virtual_machine.hpp"
#include <iostream>

namespace plzerow {

std::ostream &operator<<(std::ostream &os, OP_CODE color) {
  switch (color) {
  case OP_CODE::OP_RETURN:
    return os << "OP_RETURN";
  case OP_CODE::OP_CONSTANT:
    return os << "OP_CONSTANT";
  default:
    return os << "Unknown";
  }
}
} // namespace plzerow
