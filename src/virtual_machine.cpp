#include <functional>
#include <iostream>
#include <plzerow/chunk.hpp>
#include <plzerow/debugger.hpp>
#include <plzerow/inputhandler.hpp>
#include <plzerow/op_codes.hpp>
#include <plzerow/value.hpp>
#include <plzerow/virtual_machine.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace {

void dump_stack(std::stack<plzerow::Value> stack) {
  std::cout << "[ ";
  while (!stack.empty()) {
    auto v = stack.top();
    switch (v._type) {
    case plzerow::TOKEN::STRING:
      std::cout << v.as_string();
      break;
    case plzerow::TOKEN::INTEGER:
      std::cout << v.as_int();
      break;
    case plzerow::TOKEN::DOUBLE:
      std::cout << v.as_double();
      break;
    case plzerow::TOKEN::BOOL:
      std::cout << v.as_bool();
      break;
    }
    std::cout << " ";
    stack.pop();
  }
  std::cout << "]\n";
}

auto logical_not = [](plzerow::Value&& arg) -> plzerow::Value {
  return std::visit(
      [&](plzerow::Value&& arg) -> plzerow::Value {
        std::cout << static_cast<char>(arg._type) << "\n";
        switch (arg._type) {
        case plzerow::TOKEN::INTEGER:
          return !arg.as_int();
          break;
        case plzerow::TOKEN::DOUBLE:
          return !arg.as_double();
          break;
        case plzerow::TOKEN::BOOL:
          return !arg.as_bool();
          break;
        default:
          throw std::runtime_error("only numbers and booleans can be negated");
        }
      },
      arg._value);
};

auto logical_negate = [](plzerow::Value&& arg) -> plzerow::Value {
  return std::visit(
      [&](plzerow::Value&& arg) -> plzerow::Value {
        std::cout << static_cast<char>(arg._type) << "\n";
        switch (arg._type) {
        case plzerow::TOKEN::INTEGER:
          return -arg.as_int();
          break;
        case plzerow::TOKEN::DOUBLE:
          return -arg.as_double();
          break;
        default:
          throw std::runtime_error(
              "only numbers can have their signedness inverted");
        }
      },
      arg._value);
};

template <typename Func>
plzerow::Value binary_op(const plzerow::Value& lhs, const plzerow::Value& rhs,
                         Func f) {
  return std::visit(
      [&](const auto& lhs_val, const auto& rhs_val) -> plzerow::Value {
        using LhsType = std::decay_t<decltype(lhs_val)>;
        using RhsType = std::decay_t<decltype(rhs_val)>;

        if constexpr (std::is_same_v<Func, std::plus<>> &&
                      std::is_same_v<LhsType, std::string> &&
                      std::is_same_v<LhsType, RhsType>) {
          return lhs_val + rhs_val;
        } else {
          throw std::runtime_error("unsupported operation on string type");
        }

        if constexpr (std::is_integral_v<LhsType> &&
                      std::is_same_v<LhsType, RhsType>) {
          return f(lhs_val, rhs_val);
        } else {
          throw std::runtime_error("invalid types for binary operation");
        }
      },
      lhs._value, rhs._value);
}

auto print = [](auto&& arg) -> void { std::cout << arg << "\n"; };

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
#ifdef PLZEROW_DEBUG
    dump_stack(_stack);
    auto offset = std::distance(first_instruction, _ip);
    Debugger::disassemble_instruction(offset, _chunk);
#endif
    OP_CODE instruction = static_cast<OP_CODE>(*next());
    switch (instruction) {
    case OP_CODE::CONSTANT:
    case OP_CODE::CONSTANT_LONG:
      _stack.push(_chunk.constant(static_cast<std::size_t>(*next())));
      break;
    case OP_CODE::NEGATE:
      _stack.push(std::visit(logical_negate, pop_stack()._value));
      break;
    case OP_CODE::NOT:
      _stack.push(std::visit(logical_not, pop_stack()._value));
      break;
    case OP_CODE::MULTIPLY:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::multiplies{}));
      break;
    case OP_CODE::DIVIDE:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::divides{}));
      break;
    case OP_CODE::ADD:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::plus{}));
      break;
    case OP_CODE::SUBTRACT:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::minus{}));
      break;
    case OP_CODE::LT:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::less{}));
      break;
    case OP_CODE::LE:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::less_equal{}));
      break;
    case OP_CODE::GT:
      _stack.push(binary_op(pop_stack(), pop_stack(), std::greater{}));
      break;
    case OP_CODE::GE:
      _stack.push(binary_op(pop_stack(), pop_stack(),
                            std::greater_equal<std::int32_t>()));
      break;
    case OP_CODE::EQUAL:
      _stack.push(
          binary_op(pop_stack(), pop_stack(), std::equal_to<std::int32_t>()));
      break;
    case OP_CODE::NOT_EQUAL:
      _stack.push(binary_op(pop_stack(), pop_stack(),
                            std::not_equal_to<std::int32_t>()));
      break;
    case OP_CODE::RETURN:
      std::visit(print, _stack.top()._value);
      // std::cout << "\n";
      _stack.pop();
      return InterpretResult::OK;
    default:
      std::cout << "COMPILE_ERROR\n";
      return InterpretResult::COMPILE_ERROR;
    }
  }

  return InterpretResult::OK;
}

void VM::load_chunk(Chunk&& chunk) {
  _chunk = std::forward<Chunk>(chunk);
  _ip = _chunk.cbegin();
  std::stack<Value>().swap(_stack);
}

void VM::repl() {
  for (;;) {
    try {
      std::cout << "> ";
      auto source_code = InputHandler::read_from_repl(std::cin);
      _compiler.compile(std::move(source_code));
      load_chunk(_compiler.byte_code());
      run();
    } catch (const std::runtime_error& err) {
      std::cerr << err.what() << "\n";
    }
  }
}

void VM::runfile(const std::string& filename) {
  auto source_code = InputHandler::read_from_file(filename);
  _compiler.compile(std::move(source_code));
}

}  // namespace plzerow
