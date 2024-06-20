#include "virtual_machine.hpp"
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include "chunk.hpp"
#include "debugger.hpp"
#include "inputhandler.hpp"
#include "value.hpp"

namespace {

void dump_stack(std::stack<plzerow::Value> stack) {
  std::cout << "[ ";
  while (!stack.empty()) {
    auto v = stack.top();
    std::visit(plzerow::PrintVisitor, v);
    std::cout << " ";
    stack.pop();
  }
  std::cout << "]\n";
}

auto Negate = [](auto &&arg) -> plzerow::Value { return -arg; };

auto BinaryOp = [](const plzerow::Value &lhs, const plzerow::Value &rhs,
                   std::function<double(double, double)> f) -> plzerow::Value {
  auto lhs_v = std::visit(plzerow::Number, lhs);
  auto rhs_v = std::visit(plzerow::Number, rhs);
  /*
   * Pay close attention to the order of the two pops. Note that we assign the
   * first popped operand to b, not a. It looks backwards. When the operands
   * themselves are calculated, the left is evaluated first, then the right.
   * That means the left operand gets pushed before the right operand. So the
   * right operand will be on top of the stack. Thus, the first value we pop is
   * b.
   */
  return plzerow::Value{f(rhs_v, lhs_v)};
};

}  // namespace

namespace plzerow {

InstructionPointer VM::next() { return _ip++; }

Value VM::pop_stack() {
  auto v = _stack.top();
  _stack.pop();
  return v;
}

InterpretResult VM::run() {
  std::cout << _chunk.size() << "\n";
  auto first_instruction = _chunk.cbegin();
  for (;;) {
    dump_stack(_stack);
    auto offset = std::distance(first_instruction, _ip);
    Debugger::disassemble_instruction(offset, _chunk);
    std::uint8_t instruction;
    switch (instruction = *next()) {
    case OP_CONSTANT:
    case OP_CONSTANT_LONG:
      _stack.push(_chunk.constant(*next()));
      break;
    case OP_NEGATE:
      _stack.push(std::visit(Negate, pop_stack()));
      break;
    case OP_MULTIPLY:
      _stack.push(
          BinaryOp(pop_stack(), pop_stack(), std::multiplies<std::int32_t>()));
      break;
    case OP_DIVIDE:
      _stack.push(
          BinaryOp(pop_stack(), pop_stack(), std::divides<std::int32_t>()));
      break;
    case OP_ADD:
      _stack.push(
          BinaryOp(pop_stack(), pop_stack(), std::plus<std::int32_t>()));
      break;
    case OP_SUBTRACT:
      _stack.push(
          BinaryOp(pop_stack(), pop_stack(), std::minus<std::int32_t>()));
      break;
    case OP_RETURN:
      std::visit(PrintVisitor, _stack.top());
      std::cout << "\n";
      _stack.pop();
      return InterpretResult::OK;
    default:
      std::cout << "COMPILE_ERROR\n";
      return InterpretResult::COMPILE_ERROR;
    }
  }

  return InterpretResult::OK;
}

void VM::load_chunk(Chunk &&chunk) {
  _chunk = std::forward<Chunk>(chunk);
  _ip = _chunk.cbegin();
  std::stack<Value>().swap(_stack);
}

void VM::repl() {
  for (;;) {
    std::cout << "> ";
    auto source_code = InputHandler::read_from_repl(std::cin);
    _compiler.compile(std::move(source_code));
    load_chunk(_compiler.byte_code());
    run();
  }
}

void VM::runfile(const std::string &filename) {
  auto source_code = InputHandler::read_from_file(filename);
  _compiler.compile(std::move(source_code));
}

}  // namespace plzerow
