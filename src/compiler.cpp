#include <cstdlib>
#include <memory>
#include <plzerow/ast_nodes.hpp>
#include <plzerow/chunk.hpp>
#include <plzerow/compiler.hpp>
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

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _ast = _parser.parse();
  std::cout << to_npn(_ast) << "\n";
  _byte_code = Chunk();
  generate_byte_code(_ast);
  _byte_code.append(OP_CODE::RETURN, 0);
  return CompilerResult::OK;
}

std::string Compiler::to_npn(const std::unique_ptr<ASTNode> &node) const {
  auto char_to_str = [](TOKEN token) -> std::string {
    char c = static_cast<char>(token);
    return std::string(&c, &c + 1);
  };

  static const Visitor print_visitor{
      [this, char_to_str](const Equality &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [this, char_to_str](const Comparison &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [this, char_to_str](const Binary &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [this, char_to_str](const Term &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [this](const Grouping &expr) -> std::string {
        return to_npn(expr._expression);
      },
      [this, char_to_str](const Unary &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._right);
      },
      [this](const Primary &expr) -> std::string {
        return to_npn(expr._value);
      },
      [](const Literal &expr) -> std::string { return expr._value; },
      [this, char_to_str](const Factor &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      }};

  return node->accept(print_visitor);
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

        _byte_code.append(op, node->_linum);
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
        _byte_code.append(op, node->_linum);
      },
      [this](const Primary &expr) -> void { std::cout << "Primary\n"; },
      [this](const Grouping &expr) -> void {
        std::cout << "Grouping\n";
        evaluate(expr._expression);
      },
      [this, &node](const Literal &expr) -> void {
        std::cout << "Literal\n";
        int number = std::atoi(expr._value.c_str());
        _byte_code.append(OP_CODE::CONSTANT, Value{number}, node->_linum);
      },
      [this](const Factor &expr) -> void { std::cout << "Factor\n"; }};

  node->accept(byte_code_generator);
}

}  // namespace plzerow
