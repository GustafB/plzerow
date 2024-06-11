#pragma once

#include "value.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace plzerow {

using LineCounter = std::uint32_t;
using InstructionContainer = std::vector<std::uint8_t>;
using LinumContainer = std::vector<LineCounter>;

class Chunk {
public:
  std::size_t disassemble(const std::string &name);
  std::uint8_t append(std::uint8_t instruction, std::size_t linum);
  std::uint8_t append(std::uint8_t instruction, const Value &value,
                      std::size_t linum);
  std::uint16_t linum(std::size_t instruction_index);

private:
  std::size_t simple_instruction(const std::string &name, std::size_t offset);
  std::size_t constant_instruction(const std::string &name, std::size_t offset);
  std::size_t constant_long_instruction(const std::string &name,
                                        std::size_t offset);
  std::size_t disassemble_instruction(std::size_t offset);
  std::uint8_t append(std::uint8_t instruction);
  void add_linum(std::uint16_t linum, std::uint16_t op_size = 0);

  InstructionContainer _instructions;
  LinumContainer _linums;
  ValueArray _constants;
};

} // namespace plzerow
