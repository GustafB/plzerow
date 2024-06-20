#include "debugger.hpp"
#include <fmt/core.h>
#include <cstdint>
#include <iostream>
#include "value.hpp"
#include "virtual_machine.hpp"

namespace {

std::size_t constant_instruction_helper(const std::string &name,
                                        std::size_t offset,
                                        const plzerow::Chunk &chunk) {
  const std::uint8_t constant_index = chunk.cbegin()[offset + 1];
  const std::string result = fmt::format("{:<16} {:4} '", name, constant_index);
  std::cout << result;
  chunk.visit_constant(constant_index, plzerow::PrintVisitor);
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

}  // namespace

namespace plzerow {

std::size_t Debugger::disassemble_instruction(std::size_t offset,
                                              const Chunk &chunk) {
  std::cout << fmt::format("{:04} ", offset);
  if (offset > 0 && chunk.linum(offset) == chunk.linum(offset - 1))
    std::cout << "   | ";
  else
    std::cout << fmt::format("{:04} ", chunk.linum(offset));

  const auto instruction = static_cast<OP_CODE>(chunk.cbegin()[offset]);
  switch (instruction) {
  case OP_CODE::EQUALITY:
    return simple_instruction("EQUALITY", offset);
  case OP_CODE::LT:
    return simple_instruction("LT", offset);
  case OP_CODE::LE:
    return simple_instruction("LE", offset);
  case OP_CODE::GT:
    return simple_instruction("GT", offset);
  case OP_CODE::GE:
    return simple_instruction("GE", offset);
  case OP_CODE::ERROR:
    return simple_instruction("ERROR", offset);
  case OP_CODE::NOT:
    return simple_instruction("NOT", offset);
  case OP_CODE::DIVIDE:
    return simple_instruction("DIVIDE", offset);
  case OP_CODE::SUBTRACT:
    return simple_instruction("SUBTRACT", offset);
  case OP_CODE::MULTIPLY:
    return simple_instruction("MULTIPLY", offset);
  case OP_CODE::ADD:
    return simple_instruction("ADD", offset);
  case OP_CODE::NEGATE:
    return simple_instruction("NEGATE", offset);
  case OP_CODE::RETURN:
    return simple_instruction("RETURN", offset);
  case OP_CODE::CONSTANT:
    return constant_instruction("CONSTANT", offset, chunk);
  case OP_CODE::CONSTANT_LONG:
    return constant_long_instruction("CONSTANT_LONG", offset, chunk);
  default:
    std::cout << "unknown opcode " << static_cast<char>(instruction) << "\n";
    return offset + 1;
  }
}

std::size_t Debugger::disassemble(const std::string &name, const Chunk &chunk) {
  std::cout << "constants = " << chunk._constants.values().size()
            << " instructions = " << chunk._instructions.size()
            << " linums = " << chunk._linums.size() << "\n";

  std::cout << "== " << name << " ==\n";
  for (std::size_t offset = 0; offset < chunk._instructions.size();) {
    offset = disassemble_instruction(offset, chunk);
  }
  return 0;
}

}  // namespace plzerow
