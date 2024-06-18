#include "compiler.hpp"
#include "chunk.hpp"
#include "parser.hpp"
#include <cstdint>
#include <utility>

namespace plzerow {

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _parser.parse();
  return CompilerResult::OK;
}

// void Compiler::emit_byte(std::uint8_t byte);

} // namespace plzerow
