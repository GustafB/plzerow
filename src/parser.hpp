#pragma once

#include "lexer.hpp"
#include <vector>

namespace gbpl0 {

class Parser {
public:
  explicit Parser(Lexer lexer) : _lexer{lexer} {};
  void run();

private:
  void expect(TOKEN token);
  void next();

  Lexer _lexer;
  std::vector<Lexeme> _lexemes;
};

} // namespace gbpl0
