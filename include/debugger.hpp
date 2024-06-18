#pragma once

#include "chunk.hpp"
#include <string>

namespace plzerow {
class Debugger {
public:
  static std::size_t disassemble_instruction(std::size_t offset,
                                             const Chunk &chunk);
  static std::size_t disassemble(const std::string &name, const Chunk &chunk);
};

} // namespace plzerow
