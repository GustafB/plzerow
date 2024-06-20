#include "virtual_machine.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include "chunk.hpp"
#include "debugger.hpp"
#include "inputhandler.hpp"
#include "op_codes.hpp"
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

auto logical_not = [](auto &&arg) -> plzerow::Value { return !arg; };

auto logical_negate = [](auto &&arg) -> plzerow::Value { return -arg; };

auto binary_op = [](const plzerow::Value &lhs, const plzerow::Value &rhs,
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
  auto first_instruction = _chunk.cbegin();
  for (;;) {
    dump_stack(_stack);
    auto offset = std::distance(first_instruction, _ip);
    Debugger::disassemble_instruction(offset, _chunk);
    OP_CODE instruction = static_cast<OP_CODE>(*next());
    switch (instruction) {
    case OP_CODE::CONSTANT:
    case OP_CODE::CONSTANT_LONG:
      _stack.push(_chunk.constant(static_cast<std::size_t>(*next())));
      break;
    case OP_CODE::NEGATE:
      _stack.push(std::visit(logical_negate, pop_stack()));
      break;
    case OP_CODE::NOT:
      _stack.push(std::visit(logical_not, pop_stack()));
      break;
    case OP_CODE::MULTIPLY:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::multiplies<std::int32_t>()));
      break;
    case OP_CODE::DIVIDE:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::divides<std::int32_t>()));
      break;
    case OP_CODE::ADD:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::plus<std::int32_t>()));
      break;
    case OP_CODE::SUBTRACT:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::minus<std::int32_t>()));
      break;
    case OP_CODE::LT:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::less<std::int32_t>()));
      break;
    case OP_CODE::LE:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::less_equal<std::int32_t>()));
      break;
    case OP_CODE::GT:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::greater<std::int32_t>()));
      break;
    case OP_CODE::GE:
      _stack.push(binary_op(pop_stack(), pop_stack(),
                            std::greater_equal<std::int32_t>()));
      break;
    case OP_CODE::RETURN:
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
