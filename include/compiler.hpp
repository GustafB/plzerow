#pragma once

#include <cstdint>
#include <memory>
#include "ast_nodes.hpp"
#include "chunk.hpp"
#include "lexer.hpp"
#include "parser.hpp"

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
 public:
  Compiler() = default;

  CompilerResult compile(std::vector<char> &&source_code);
  Chunk byte_code() { return _byte_code; }

 private:
  std::string to_npn(const std::unique_ptr<ASTNode> &node) const;
  void generate_byte_code(const std::unique_ptr<ASTNode> &expr);

  template <typename... Exprs>
  std::string parenthesize(std::string_view name, Exprs &...exprs) const;

  void evaluate(const std::unique_ptr<ASTNode> &expr);

  std::unique_ptr<ASTNode> _ast;
  Chunk _byte_code;
  Parser _parser;
  Lexer _lexer;
};

template <typename... Exprs>
std::string Compiler::parenthesize(std::string_view name,
                                   Exprs &...exprs) const {
  std::stringstream ss;
  ss << "( " << name;
  ((ss << " " << to_npn(exprs)), ...);
  ss << " )";
  return ss.str();
}

}  // namespace plzerow
