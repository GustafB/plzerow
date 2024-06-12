#include "debugger.hpp"
#include "value.hpp"
#include "virtual_machine.hpp"
#include <fmt/core.h>
#include <iostream>

namespace {

std::size_t constant_instruction_helper(const std::string &name,
                                        std::size_t offset,
                                        const plzerow::Chunk &chunk) {
  auto constant_index = chunk.cbegin()[offset + 1];
  auto result = fmt::format("{:<16} {:4} '", name, constant_index);
  std::cout << result;
  chunk.visit_constant(constant_index, std::forward<plzerow::PrintVisitor>(
                                           plzerow::PrintVisitor(std::cout)));
  std::cout << "\n";
  return offset + 2;
}

std::size_t simple_instruction(const std::string &name, std::size_t offset) {
  std::cout << name << "\n";
  return offset + 1;
}

std::size_t constant_instruction(const std::string &name, std::size_t offset,
                                 const plzerow::Chunk &chunk) {
  return constant_instruction_helper(name, offset, chunk);
}

std::size_t constant_long_instruction(const std::string &name,
                                      std::size_t offset,
                                      const plzerow::Chunk &chunk) {
  return constant_instruction_helper(name, offset, chunk);
}

} // namespace

namespace plzerow {

std::size_t disassemble_instruction(std::size_t offset, const Chunk &chunk) {
  std::cout << fmt::format("{:04} ", offset);
  if (offset > 0 && chunk.linum(offset) == chunk.linum(offset - 1))
    std::cout << "   | ";
  else
    std::cout << fmt::format("{:04} ", chunk.linum(offset));

  auto instruction = chunk.cbegin()[offset];
  switch (chunk.cbegin()[offset]) {
  case OP_RETURN:
    return simple_instruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", offset, chunk);
  case OP_CONSTANT_LONG:
    return constant_long_instruction("OP_CONSTANT_LONG", offset, chunk);
  default:
    std::cout << "unknown opcode " << instruction << "\n";
    return offset + 1;
  }
}

std::size_t disassemble(const std::string &name, const Chunk &chunk) {
  std::cout << "constants = " << chunk._constants.values().size()
            << " instructions = " << chunk._instructions.size()
            << " linums = " << chunk._linums.size() << "\n";

  std::cout << "== " << name << " ==\n";
  for (std::size_t offset = 0; offset < chunk._instructions.size();) {
    offset = disassemble_instruction(offset, chunk);
  }
  return 0;
}

} // namespace plzerow
