#include "chunk.hpp"
#include <cstdint>
#include "op_codes.hpp"
#include "value.hpp"

namespace plzerow {

std::size_t Chunk::append(std::uint8_t instruction) {
  _instructions.push_back(static_cast<std::uint8_t>(instruction));
  return _instructions.size();
}

std::size_t Chunk::append(OP_CODE instruction, std::size_t linum) {
  add_linum(linum, 1);
  return append(static_cast<std::uint8_t>(instruction));
}

std::size_t Chunk::append(OP_CODE instruction, const Value &value,
                          std::size_t linum) {
  auto constant_index = _constants.append(value);
  add_linum(linum, 2);
  append(static_cast<uint8_t>(instruction));
  return append(static_cast<std::uint8_t>(constant_index)) - 1;
}

void Chunk::add_linum(std::uint16_t linum, std::uint16_t instruction_size) {
  auto combine = [](std::uint16_t linum, std::uint16_t instruction_size) {
    std::uint32_t combined = 0 | static_cast<std::uint32_t>(linum);
    combined |= static_cast<std::uint32_t>(instruction_size << 16);
    return combined;
  };

  if (_linums.size() == 0) {
    std::uint32_t combined = combine(linum, instruction_size);
    _linums.push_back(combined);
    return;
  }

  auto &line_info = _linums.back();
  auto line_number = static_cast<std::uint16_t>(line_info & 0xFFFF);
  auto op_count = static_cast<std::uint16_t>((line_info >> 16) & 0xFFFF);
  if (line_number == linum) {
    line_info = combine(line_number, op_count + instruction_size);
  } else {
    _linums.push_back(combine(linum, instruction_size));
  }
}

std::uint16_t Chunk::linum(std::size_t instruction_index) const {
  std::size_t index = 0;
  for (auto line_info : _linums) {
    auto line_number = static_cast<std::uint16_t>(line_info & 0xFFFF);
    auto op_count = static_cast<std::uint16_t>((line_info >> 16) & 0xFFFF);
    index += op_count;
    if (index > instruction_index) {
      return line_number;
    }
  }
  return 0;
}

InstructionPointer Chunk::cbegin() const { return _instructions.cbegin(); }

Value Chunk::constant(std::size_t index) const {
  return _constants.values()[index];
}

}  // namespace plzerow
