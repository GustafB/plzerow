#pragma once

#include "ast_nodes.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <cstdint>
#include <memory>

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
public:
  Compiler() = default;

  CompilerResult compile(std::vector<char> &&source_code);

private:
  std::string print(const std::unique_ptr<ASTNode> &node) const;
  void emit_byte(std::uint8_t byte);
  void emit_bytes(std::uint8_t byte1, std::uint8_t byte2);
  void emit_return();

  template <typename... Exprs>
  std::string parenthesize(std::string_view name, Exprs &...exprs) const {
    std::stringstream ss;
    ss << "( " << name;
    ((ss << " " << print(exprs)), ...);
    ss << " )";
    return ss.str();
  }

  std::unique_ptr<ASTNode> _ast;
  Parser _parser;
  Lexer _lexer;
};

} // namespace plzerow
