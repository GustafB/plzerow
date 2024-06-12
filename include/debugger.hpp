#pragma once

#include "chunk.hpp"
#include <string>

namespace plzerow {

class Debugger {
public:
  Debugger(Chunk chunk) : _chunk{chunk} {}
  std::size_t disassemble(const std::string &name) const;

private:
  std::size_t simple_instruction(const std::string &name,
                                 std::size_t offset) const;
  std::size_t constant_instruction(const std::string &name,
                                   std::size_t offset) const;
  std::size_t constant_long_instruction(const std::string &name,
                                        std::size_t offset) const;
  std::size_t disassemble_instruction(std::size_t offset) const;

  Chunk _chunk;
};

} // namespace plzerow
