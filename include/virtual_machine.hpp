#pragma once

#include <cstdint>
#include <stack>
#include <utility>
#include "chunk.hpp"
#include "compiler.hpp"
#include "value.hpp"

namespace plzerow {

enum OP_CODE : std::uint8_t {
  OP_RETURN,
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_NEGATE,
  OP_ADD,
  OP_MULTIPLY,
  OP_SUBTRACT,
  OP_DIVIDE,
  OP_EQUALITY,
  OP_LT,
  OP_LE,
  OP_GT,
  OP_GE,
  OP_ERROR
};

enum class InterpretResult { OK, COMPILE_ERROR, RUNTIME_ERROR };

class VM {
 public:
  VM() = default;
  VM(Chunk &&chunk) : _chunk{std::forward<Chunk>(chunk)} {
    _ip = _chunk.cbegin();
  };
  InterpretResult run();

  void repl();
  void runfile(const std::string &filename);

 private:
  InstructionPointer next();
  Value pop_stack();
  void load_chunk(Chunk &&chunk);

  InstructionPointer _ip;
  Chunk _chunk;
  std::stack<Value> _stack;
  Compiler _compiler;
};

}  // namespace plzerow
