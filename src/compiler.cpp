#include "compiler.hpp"
#include "parser.hpp"
#include <utility>

namespace plzerow {

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _parser.parse();
  return CompilerResult::OK;
}

} // namespace plzerow
