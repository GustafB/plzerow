#pragma once

#include "value.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace gbpl0 {

class Chunk {
public:
  std::size_t disassemble(const std::string &name);
  std::uint8_t append(std::uint8_t byte, std::size_t linum);
  std::uint8_t append(std::uint8_t byte, const Value &value, std::size_t linum);
  std::uint16_t linum(std::size_t instruction_index);

private:
  std::size_t simple_instruction(const std::string &name, std::size_t offset);
  std::size_t constant_instruction(const std::string &name, std::size_t offset);
  std::size_t disassemble_instruction(std::size_t offset);
  std::uint8_t append(std::uint8_t byte);
  void add_linum(std::uint16_t linum, std::uint16_t op_size = 0);

  std::vector<std::uint8_t> _instructions;
  std::vector<std::uint32_t> _linums;
  ValueArray _constants;
};

} // namespace gbpl0
