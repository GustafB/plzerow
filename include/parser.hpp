#pragma once

#include "lexer.hpp"
#include <vector>

namespace gbpl0 {

class Parser {
public:
  explicit Parser(Lexer &&lexer) : _lexer{std::move(lexer)} {};
  void run();

  void read_tokens();

private:
  const Lexeme &current() const;
  void expect(TOKEN expected_token);
  void next();

  void block();
  void statement();
  void expression();
  void condition();
  void term();
  void factor();

  void parse_error(const std::string &err) const;

  Lexer _lexer;
  std::vector<Lexeme> _lexemes;
};

} // namespace gbpl0
