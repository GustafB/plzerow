#include "virtual_machine.hpp"
#include "chunk.hpp"
#include "debugger.hpp"
#include "value.hpp"
#include <functional>
#include <iostream>
#include <variant>

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
  return plzerow::Value{f(lhs_v, rhs_v)};
};

} // namespace

namespace plzerow {

InstructionPointer VM::next() { return _ip++; }

Value VM::pop() {
  auto v = _stack.top();
  _stack.pop();
  return v;
}

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
    case OP_NEGATE: {
      auto v = pop();
      _stack.push(std::visit(Negate, v));
    } break;
    case OP_MULTIPLY: {
      _stack.push(BinaryOp(pop(), pop(), std::multiplies<std::int32_t>()));
    } break;
    case OP_DIVIDE: {
      _stack.push(BinaryOp(pop(), pop(), std::divides<std::int32_t>()));
    } break;
    case OP_ADD: {
      _stack.push(BinaryOp(pop(), pop(), std::plus<std::int32_t>()));
    } break;
    case OP_SUBTRACT: {
      _stack.push(BinaryOp(pop(), pop(), std::minus<std::int32_t>()));
    } break;
    case OP_RETURN:
      std::visit(PrintVisitor, _stack.top());
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
