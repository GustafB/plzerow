#pragma once

#include "token.hpp"
#include <functional>
#include <vector>

namespace {

using Tokens = std::vector<plzerow::Token>;
using TokenIterator = Tokens::const_iterator;
using TokenGenerator = std::function<plzerow::Token()>;

} // namespace

namespace plzerow {

class Parser {
public:
  Parser();
  Parser(TokenGenerator &&next_token);
  void parse();

private:
  const Token &current() const;
  const Token &previous() const;
  void expect(TOKEN expected_token);
  void next();

  void block();
  void statement();
  void expression();
  void condition();
  void term();
  void factor();

  void parse_error(const std::string &err) const;

  std::function<Token()> _next_token;
  Token _current;
  Token _previous;
};

} // namespace plzerow
