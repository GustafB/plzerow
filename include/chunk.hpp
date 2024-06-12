#pragma once

#include "value.hpp"
#include <cstdint>
#include <vector>

namespace plzerow {

using LineCounter = std::uint32_t;
using InstructionContainer = std::vector<std::uint8_t>;
using LinumContainer = std::vector<LineCounter>;
using InstructionPointer = InstructionContainer::const_iterator;

class Chunk {
  friend std::size_t disassemble(const std::string &name, const Chunk &chunk);

public:
  std::uint8_t append(std::uint8_t instruction, std::size_t linum);
  std::uint8_t append(std::uint8_t instruction, const Value &value,
                      std::size_t linum);

  InstructionPointer cbegin() const;

  template <typename Visitor>
  auto visit_constant(std::size_t index, Visitor &&visitor) const;

  Value constant(std::size_t index) const;

  std::uint16_t linum(std::size_t instruction_index) const;

private:
  std::uint8_t append(std::uint8_t instruction);
  void add_linum(std::uint16_t linum, std::uint16_t op_size);

  InstructionContainer _instructions;
  LinumContainer _linums;
  ValueArray _constants;
};

template <typename Visitor>
auto Chunk::visit_constant(std::size_t index, Visitor &&visitor) const {
  return _constants.visit(index, std::forward<Visitor>(visitor));
}

} // namespace plzerow
