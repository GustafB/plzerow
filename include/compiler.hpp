#pragma once

#include "ast_nodes.hpp"
#include "chunk.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <cstdint>
#include <memory>

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
public:
  Compiler() = default;

  Chunk compile(std::vector<char> &&source_code);

private:
  std::string to_npn(const std::unique_ptr<ASTNode> &node) const;
  Chunk generate_byte_code();

  void emit_byte(std::uint8_t byte);
  void emit_bytes(std::uint8_t byte1, std::uint8_t byte2);
  void emit_return();

  template <typename... Exprs>
  std::string parenthesize(std::string_view name, Exprs &...exprs) const;

  std::unique_ptr<ASTNode> _ast;
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

} // namespace plzerow
