#include "virtual_machine.hpp"
#include "chunk.hpp"
#include "debugger.hpp"
#include "value.hpp"
#include <iostream>
#include <variant>

namespace {

void dump_stack(std::stack<plzerow::Value> stack) {
  std::cout << "[ ";
  while (!stack.empty()) {
    auto v = stack.top();
    std::visit(plzerow::PrintVisitor(std::cout), v);
    std::cout << " ";
    stack.pop();
  }
  std::cout << "]\n";
}

} // namespace

namespace plzerow {

InstructionPointer VM::next() { return _ip++; }

InterpretResult VM::run() {
  for (;;) {
    dump_stack(_stack);
    disassemble_instruction(_ip - _chunk.cbegin(), _chunk);
    std::uint8_t instruction;
    switch (instruction = *next()) {
    case OP_CONSTANT:
    case OP_CONSTANT_LONG:
      _stack.push(_chunk.constant(*next()));
      break;
    case OP_RETURN:
      std::visit(plzerow::PrintVisitor(std::cout), _stack.top());
      _stack.pop();
      return InterpretResult::OK;
    default:
      std::cout << "COMPILE_ERROR\n";
      return InterpretResult::COMPILE_ERROR;
    }
  }

  return InterpretResult::OK;
}

} // namespace plzerow
