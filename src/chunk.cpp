#include "chunk.hpp"
#include "value.hpp"
#include "virtual_machine.hpp"
#include <cstdint>
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

std::size_t Chunk::simple_instruction(const std::string &name,
                                      std::size_t offset) {
  std::cout << name << "\n";
  return offset + 1;
}

std::size_t Chunk::constant_instruction(const std::string &name,
                                        std::size_t offset) {
  return constant_instruction_helper<double>(name, offset, _instructions,
                                             _constants);
}

std::size_t Chunk::constant_long_instruction(const std::string &name,
                                             std::size_t offset) {
  return constant_instruction_helper<std::uint32_t>(name, offset, _instructions,
                                                    _constants);
}

std::size_t Chunk::disassemble_instruction(std::size_t offset) {
  std::cout << fmt::format("{:04} ", offset);
  if (offset > 0 && linum(offset) == linum(offset - 1))
    std::cout << "   | ";
  else
    std::cout << fmt::format("{:04} ", linum(offset));

  auto instruction = _instructions[offset];
  switch (_instructions[offset]) {
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

std::size_t Chunk::disassemble(const std::string &name) {
  std::cout << "constants = " << _constants.values().size()
            << " instructions = " << _instructions.size()
            << " linums = " << _linums.size() << "\n";

  std::cout << "== " << name << " ==\n";
  for (std::size_t offset = 0; offset < _instructions.size();) {
    offset = disassemble_instruction(offset);
  }
  return 0;
}

std::uint8_t Chunk::append(std::uint8_t instruction) {
  _instructions.push_back(instruction);
  return _instructions.size();
}

std::uint8_t Chunk::append(std::uint8_t instruction, std::size_t linum) {
  add_linum(linum);
  return append(instruction);
}

std::uint8_t Chunk::append(std::uint8_t instruction, const Value &value,
                           std::size_t linum) {
  auto constant_index = _constants.append(value);
  add_linum(linum, 1);
  append(instruction);
  return append(constant_index) - 1;
}

void Chunk::add_linum(std::uint16_t linum, std::uint16_t op_size) {
  auto combine = [linum](std::uint16_t op_count) {
    std::uint32_t combined = 0;
    combined |= static_cast<std::uint32_t>(linum);
    combined |= static_cast<std::uint32_t>(op_count << 16);
    return combined;
  };

  if (_linums.size() == 0) {
    std::uint32_t combined = linum;
    _linums.push_back(combined);
  }

  auto &line_info = _linums.back();
  auto line_number = static_cast<std::uint16_t>(line_info & 0xFFFF);
  auto op_count = static_cast<std::uint16_t>((line_info >> 16) & 0xFFFF);
  if (line_number == linum) {
    line_info = combine(++op_count + op_size);
  } else {
    _linums.push_back(combine(0x0001));
  }
}

std::uint16_t Chunk::linum(std::size_t instruction_index) {
  std::size_t index = 0;
  for (auto line_info : _linums) {
    auto line_number = static_cast<std::uint16_t>(line_info & 0xFFFF);
    auto op_count = static_cast<std::uint16_t>((line_info >> 16) & 0xFFFF);
    index += op_count;
    if (index >= instruction_index + 1) {
      return line_number;
    }
  }
  return 0;
}

} // namespace plzerow
