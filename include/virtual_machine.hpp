#pragma once

#include "chunk.hpp"
#include "value.hpp"
#include <cstdint>
#include <stack>
#include <utility>

namespace plzerow {

enum OP_CODE : std::uint8_t {
  OP_RETURN,
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_NEGATE,
  OP_ADD,
  OP_MULTIPLY,
  OP_SUBTRACT,
  OP_DIVIDE
};

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

  Value pop_stack();

  InstructionPointer _ip;
  Chunk _chunk;
  std::stack<Value> _stack;
};

} // namespace plzerow
