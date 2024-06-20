#pragma once

#include <memory>
#include <plzerow/ast_nodes.hpp>
#include <plzerow/chunk.hpp>
#include <plzerow/lexer.hpp>
#include <plzerow/parser.hpp>

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
 public:
  Compiler() = default;

  CompilerResult compile(std::vector<char> &&source_code);
  Chunk byte_code() { return _byte_code; }

 private:
  void emit_instruction(OP_CODE instruction, std::size_t linum);
  void emit_constant(OP_CODE instruction, Value &&value, std::size_t linum);

  std::string to_npn(const std::unique_ptr<ASTNode> &node) const;
  void generate_byte_code(const std::unique_ptr<ASTNode> &expr);

  void evaluate(const std::unique_ptr<ASTNode> &expr);

  std::unique_ptr<ASTNode> _ast;
  Chunk _byte_code;
  Parser _parser;
  Lexer _lexer;
};

}  // namespace plzerow
