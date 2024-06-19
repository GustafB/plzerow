#include "compiler.hpp"
#include "ast_nodes.hpp"
#include "chunk.hpp"
#include "parser.hpp"
#include "value.hpp"
#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>

namespace plzerow {

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _ast = _parser.parse();
  std::cout << print(_ast) << "\n";
  return CompilerResult::OK;
}

std::string Compiler::print(const std::unique_ptr<ASTNode> &node) const {
  auto char_to_str = [](TOKEN token) -> std::string {
    char c = static_cast<char>(token);
    return std::string(&c, &c + 1);
  };

  static const Visitor printVisitor{
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
      [this, char_to_str](const Unary &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      },
      [this](const Primary &expr) -> std::string { return print(expr._value); },
      [](const Literal &expr) -> std::string { return expr._value; },
      [this, char_to_str](const Factor &expr) -> std::string {
        return parenthesize(char_to_str(expr._op), expr._left, expr._right);
      }};

  return node->accept(printVisitor);
}

// void Compiler::emit_byte(std::uint8_t byte);

} // namespace plzerow
