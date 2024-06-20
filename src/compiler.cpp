#include <cstdlib>
#include <memory>
#include <plzerow/ast_nodes.hpp>
#include <plzerow/chunk.hpp>
#include <plzerow/compiler.hpp>
#include <plzerow/debugger.hpp>
#include <plzerow/op_codes.hpp>
#include <plzerow/parser.hpp>
#include <plzerow/value.hpp>
#include <utility>

namespace {

enum class PRECEDENCE {
  NONE,
  ASSIGNMENT,  // =
  EQUALITY,    // == !=
  COMPARISON,  // < > <= >=
  TERM,        // + -
  FACTOR,      // * /
  UNARY,       // ! -
  CALL,        // call
  PRIMARY,
  LITERAL,
};

}

namespace plzerow {

void Compiler::emit_instruction(plzerow::OP_CODE instruction,
                                std::size_t linum) {
  _byte_code.append(instruction, linum);
}

void Compiler::emit_constant(plzerow::OP_CODE instruction,
                             plzerow::Value &&constant, std::size_t linum) {
  _byte_code.append(instruction, std::forward<Value>(constant), linum);
}

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _ast = _parser.parse();
#ifdef PLZEROW_DEBUG
  std::cout << Debugger::ast_to_npn(_ast) << "\n";
#endif
  _byte_code = Chunk();
  generate_byte_code(_ast);
  emit_instruction(OP_CODE::RETURN, 0);
  return CompilerResult::OK;
}

void Compiler::evaluate(const std::unique_ptr<ASTNode> &expr) {
  generate_byte_code(expr);
}

void Compiler::generate_byte_code(const std::unique_ptr<ASTNode> &node) {
  static const Visitor byte_code_generator{
      [this](const Equality &expr) -> void { std::cout << "Equality\n"; },
      [this](const Comparison &expr) -> void { std::cout << "Comparison\n"; },
      [this, &node](const Binary &expr) -> void {
        std::cout << "Binary\n";
        evaluate(expr._left);
        evaluate(expr._right);
        OP_CODE op;
        switch (expr._op) {
        case TOKEN::PLUS:
          op = OP_CODE::ADD;
          break;
        case TOKEN::MINUS:
          op = OP_CODE::SUBTRACT;
          break;
        case TOKEN::MULTIPLY:
          op = OP_CODE::MULTIPLY;
          break;
        case TOKEN::DIVIDE:
          op = OP_CODE::DIVIDE;
          break;
        case TOKEN::GT:
          op = OP_CODE::GT;
          break;
        case TOKEN::GE:
          op = OP_CODE::GE;
          break;
        case TOKEN::LE:
          op = OP_CODE::LE;
          break;
        case TOKEN::LT:
          op = OP_CODE::LT;
          break;
        case TOKEN::BANG_EQUAL:
          op = OP_CODE::NOT_EQUAL;
          break;
        case TOKEN::EQUAL_EQUAL:
          op = OP_CODE::EQUAL;
          break;
        default:
          return;
        }
        emit_instruction(op, node->_linum);
      },
      [this](const Term &expr) -> void { std::cout << "Term\n"; },
      [this, &node](const Unary &expr) -> void {
        std::cout << "Unary\n";
        evaluate(expr._right);
        OP_CODE op;
        switch (expr._op) {
        case TOKEN::NOT:
          op = OP_CODE::NOT;
          break;
        case TOKEN::MINUS:
          op = OP_CODE::NEGATE;
          break;
        default:
          return;
        }
        emit_instruction(op, node->_linum);
      },
      [this](const Primary &expr) -> void { std::cout << "Primary\n"; },
      [this](const Grouping &expr) -> void {
        std::cout << "Grouping\n";
        evaluate(expr._expression);
      },
      [this, &node](const Literal &expr) -> void {
        std::cout << "Literal\n";
        int number = std::atoi(expr._value.c_str());
        emit_constant(OP_CODE::CONSTANT, Value{number}, node->_linum);
      },
      [this](const Factor &expr) -> void { std::cout << "Factor\n"; }};

  node->accept(byte_code_generator);
}

}  // namespace plzerow
