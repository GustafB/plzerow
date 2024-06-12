#include "chunk.hpp"
#include "value.hpp"
#include <cstdint>

namespace plzerow {

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

std::uint16_t Chunk::linum(std::size_t instruction_index) const {
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

InstructionPointer Chunk::instructions() const { return _instructions.begin(); }

} // namespace plzerow
