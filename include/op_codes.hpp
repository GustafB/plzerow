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
  EQUAL,
  NOT_EQUAL,
  LT,
  LE,
  GT,
  GE,
  ERROR,
  NOT,
};

}
