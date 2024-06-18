#pragma once

#include "lexer.hpp"
#include "parser.hpp"
#include <cstdint>

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
public:
  Compiler() = default;

  CompilerResult compile(std::vector<char> &&source_code);

private:
  void emit_byte(std::uint8_t byte);
  void emit_bytes(std::uint8_t byte1, std::uint8_t byte2);
  void emit_return();

  Parser _parser;
  Lexer _lexer;
};

} // namespace plzerow
