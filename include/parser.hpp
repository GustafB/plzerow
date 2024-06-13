#pragma once

#include "lexeme.hpp"
#include <iostream>
#include <vector>

namespace {

using Tokens = std::vector<plzerow::Lexeme>;
using TokenIterator = Tokens::const_iterator;

} // namespace

namespace plzerow {

class Parser {
public:
  Parser() = default;
  Parser(std::vector<Lexeme> &&tokens)
      : _tokens{std::forward<std::vector<Lexeme>>(tokens)},
        _current{_tokens.cbegin()} {
    std::cout << "total tokens: " << _tokens.size() << "\n";
  };

  void parse();

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

  Tokens _tokens;
  TokenIterator _current;
};

} // namespace plzerow
