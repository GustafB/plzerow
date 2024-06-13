#pragma once

#include "lexer.hpp"
#include "parser.hpp"

namespace plzerow {

enum class CompilerResult { OK, LexicalError, ParseError };

class Compiler {
public:
  Compiler() = default;

  CompilerResult compile(std::vector<char> &&source_code);

private:
  Parser _parser;
  Lexer _lexer;
};

} // namespace plzerow
