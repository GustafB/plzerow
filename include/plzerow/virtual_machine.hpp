#pragma once

#include <plzerow/chunk.hpp>
#include <plzerow/compiler.hpp>
#include <plzerow/value.hpp>
#include <stack>
#include <utility>

namespace plzerow {

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