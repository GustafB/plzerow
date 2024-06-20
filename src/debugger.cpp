#include <fmt/core.h>
#include <cstdint>
#include <iostream>
#include <plzerow/debugger.hpp>
#include <plzerow/value.hpp>
#include <sstream>

namespace {

template <typename... Exprs>
std::string parenthesize(std::string_view name, Exprs &...exprs) {
  std::stringstream ss;
  ss << "( " << name;
  ((ss << " " << plzerow::Debugger::ast_to_npn(exprs)), ...);
  ss << " )";
  return ss.str();
}

std::size_t constant_instruction_helper(const std::string &name,
                                        std::size_t offset,
                                        const plzerow::Chunk &chunk) {
  const std::uint8_t constant_index = chunk.cbegin()[offset + 1];
  const std::string result = fmt::format("{:<16} {:4} '", name, constant_index);
  std::cout << result;
  chunk.visit_constant(constant_index, plzerow::PrintVisitor);
  std::cout << "\n";
  return offset + 2;
}

std::size_t simple_instruction(const std::string &name, std::size_t offset) {
  std::cout << name << "\n";
  return offset + 1;
}

std::size_t constant_instruction(const std::string &name, std::size_t offset,
                                 const plzerow::Chunk &chunk) {
  return constant_instruction_helper(name, offset, chunk);
}

std::size_t constant_long_instruction(const std::string &name,
                                      std::size_t offset,
                                      const plzerow::Chunk &chunk) {
  return constant_instruction_helper(name, offset, chunk);
}

}  // namespace

namespace plzerow {

#ifdef PLZEROW_DEBUG

std::size_t Debugger::disassemble_instruction(std::size_t offset,
                                              const Chunk &chunk) {
  std::cout << fmt::format("{:04} ", offset);
  if (offset > 0 && chunk.linum(offset) == chunk.linum(offset - 1))
    std::cout << "   | ";
  else
    std::cout << fmt::format("{:04} ", chunk.linum(offset));

  const auto instruction = static_cast<OP_CODE>(chunk.cbegin()[offset]);
  switch (instruction) {
  case OP_CODE::EQUAL:
    return simple_instruction("EQUAL", offset);
  case OP_CODE::NOT_EQUAL:
    return simple_instruction("NOT_EQUAL", offset);
  case OP_CODE::LT:
    return simple_instruction("LT", offset);
  case OP_CODE::LE:
    return simple_instruction("LE", offset);
  case OP_CODE::GT:
    return simple_instruction("GT", offset);
  case OP_CODE::GE:
    return simple_instruction("GE", offset);
  case OP_CODE::ERROR:
    return simple_instruction("ERROR", offset);
  case OP_CODE::NOT:
    return simple_instruction("NOT", offset);
  case OP_CODE::DIVIDE:
    return simple_instruction("DIVIDE", offset);
  case OP_CODE::SUBTRACT:
    return simple_instruction("SUBTRACT", offset);
  case OP_CODE::MULTIPLY:
    return simple_instruction("MULTIPLY", offset);
  case OP_CODE::ADD:
    return simple_instruction("ADD", offset);
  case OP_CODE::NEGATE:
    return simple_instruction("NEGATE", offset);
  case OP_CODE::RETURN:
    return simple_instruction("RETURN", offset);
  case OP_CODE::CONSTANT:
    return constant_instruction("CONSTANT", offset, chunk);
  case OP_CODE::CONSTANT_LONG:
    return constant_long_instruction("CONSTANT_LONG", offset, chunk);
  default:
    std::cout << "unknown opcode " << static_cast<char>(instruction) << "\n";
    return offset + 1;
  }
}

std::size_t Debugger::disassemble(const std::string &name, const Chunk &chunk) {
  std::cout << "constants = " << chunk._constants.values().size()
            << " instructions = " << chunk._instructions.size()
            << " linums = " << chunk._linums.size() << "\n";

  std::cout << "== " << name << " ==\n";
  for (std::size_t offset = 0; offset < chunk._instructions.size();) {
    offset = disassemble_instruction(offset, chunk);
  }
  return 0;
}

std::string Debugger::ast_to_npn(const std::unique_ptr<ASTNode> &node) {
  auto char_to_str = [](TOKEN token) -> std::string {
    char c = static_cast<char>(token);
    return std::string(&c, &c + 1);
  };

  static const Visitor print_visitor{
      [char_to_str](const Equality &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [char_to_str](const Comparison &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [char_to_str](const Binary &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [char_to_str](const Term &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [](const Grouping &expr) -> std::string {
        return ast_to_npn(expr._expression);
      },
      [char_to_str](const Unary &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._right);
      },
      [](const Primary &expr) -> std::string {
        return ast_to_npn(expr._value);
      },
      [](const Literal &expr) -> std::string { return expr._value; },
      [char_to_str](const Factor &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      }};

  return node->accept(print_visitor);
}

#endif

}  // namespace plzerow
