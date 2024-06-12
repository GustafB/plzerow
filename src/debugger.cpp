#include "debugger.hpp"
#include "virtual_machine.hpp"
#include <fmt/core.h>
#include <iostream>

namespace {

template <typename T>
std::size_t
constant_instruction_helper(const std::string &name, std::size_t offset,
                            const plzerow::InstructionContainer &instructions,
                            const plzerow::ValueArray &constants) {
  auto constant_index = instructions[offset + 1];
  auto result = fmt::format("{:<16} {:4} '", name, constant_index);
  std::cout << result << constants.at<T>(constant_index) << "\n";
  return offset + 2;
}

} // namespace

namespace plzerow {

std::size_t Debugger::simple_instruction(const std::string &name,
                                         std::size_t offset) const {
  std::cout << name << "\n";
  return offset + 1;
}

std::size_t Debugger::constant_instruction(const std::string &name,
                                           std::size_t offset) const {
  return constant_instruction_helper<double>(name, offset, _chunk._instructions,
                                             _chunk._constants);
}

std::size_t Debugger::constant_long_instruction(const std::string &name,
                                                std::size_t offset) const {
  return constant_instruction_helper<std::uint32_t>(
      name, offset, _chunk._instructions, _chunk._constants);
}

std::size_t Debugger::disassemble_instruction(std::size_t offset) const {
  std::cout << fmt::format("{:04} ", offset);
  if (offset > 0 && _chunk.linum(offset) == _chunk.linum(offset - 1))
    std::cout << "   | ";
  else
    std::cout << fmt::format("{:04} ", _chunk.linum(offset));

  auto instruction = _chunk._instructions[offset];
  switch (_chunk._instructions[offset]) {
  case OP_RETURN:
    return simple_instruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", offset);
  case OP_CONSTANT_LONG:
    return constant_long_instruction("OP_CONSTANT_LONG", offset);
  default:
    std::cout << "unknown opcode " << instruction << "\n";
    return offset + 1;
  }
}

std::size_t Debugger::disassemble(const std::string &name) const {
  std::cout << "constants = " << _chunk._constants.values().size()
            << " instructions = " << _chunk._instructions.size()
            << " linums = " << _chunk._linums.size() << "\n";

  std::cout << "== " << name << " ==\n";
  for (std::size_t offset = 0; offset < _chunk._instructions.size();) {
    offset = disassemble_instruction(offset);
  }
  return 0;
}

} // namespace plzerow
