#include "compiler.hpp"
#include "parser.hpp"
#include <utility>

namespace plzerow {

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  auto tokens = _lexer.tokenize();
  _parser = Parser(std::move(tokens));
  _parser.parse();
  return CompilerResult::OK;
}

} // namespace plzerow
