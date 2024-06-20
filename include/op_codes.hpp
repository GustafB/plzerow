#pragma once

#include <cstdint>

namespace plzerow {

enum class OP_CODE : std::uint8_t {
  RETURN,
  CONSTANT,
  CONSTANT_LONG,
  NEGATE,
  ADD,
  MULTIPLY,
  SUBTRACT,
  DIVIDE,
  EQUALITY,
  LT,
  LE,
  GT,
  GE,
  ERROR,
  NOT,
};

}
