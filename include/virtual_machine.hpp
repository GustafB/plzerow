#pragma once

#include "chunk.hpp"
#include <cstdint>
#include <utility>

namespace plzerow {

enum OP_CODE : std::uint8_t { OP_RETURN, OP_CONSTANT, OP_CONSTANT_LONG };

enum class InterpretResult { OK, COMPILE_ERROR, RUNTIME_ERROR };

class VM {
public:
  VM(Chunk &&chunk) : _chunk{std::forward<Chunk>(chunk)} {
    _ip = _chunk.cbegin();
  };
  InterpretResult run();

private:
  InstructionPointer next();
  std::uint8_t next_test();

  InstructionPointer _ip;
  Chunk _chunk;
};

} // namespace plzerow
