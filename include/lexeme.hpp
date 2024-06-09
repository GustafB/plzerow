#pragma once

#include "token.hpp"
#include <string>
#include <utility>

namespace gbpl0 {

class Lexeme {
public:
  Lexeme(TOKEN token) : _token{token}, _literal{""} {}
  Lexeme(TOKEN token, const std::string &literal)
      : _token{token}, _literal{literal} {}
  Lexeme(TOKEN token, std::string &&literal)
      : _token{token}, _literal{std::move(literal)} {}

  Lexeme(const Lexeme &) = default;
  Lexeme(Lexeme &&) noexcept = default;
  Lexeme &operator=(const Lexeme &) = default;
  Lexeme &operator=(Lexeme &&) noexcept = default;
  ~Lexeme() = default;

  TOKEN token() const;
  std::string literal() const;

private:
  TOKEN _token;
  std::string _literal;
};

} // namespace gbpl0
