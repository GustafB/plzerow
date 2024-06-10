#pragma once

#include "token.hpp"
#include <ostream>
#include <string>
#include <utility>

namespace gbpl0 {

class Lexeme {
public:
  Lexeme(TOKEN token, std::size_t linum, std::size_t token_start)
      : _token{token}, _literal{""}, _linum{linum}, _token_start{token_start} {}
  Lexeme(TOKEN token, const std::string &literal, std::size_t linum,
         std::size_t token_start)
      : _token{token}, _literal{literal}, _linum{linum},
        _token_start{token_start} {}
  Lexeme(TOKEN token, std::string &&literal, std::size_t linum,
         std::size_t token_start)
      : _token{token}, _literal{std::move(literal)}, _linum{linum},
        _token_start{token_start} {}

  Lexeme(const Lexeme &) = default;
  Lexeme(Lexeme &&) noexcept = default;
  Lexeme &operator=(const Lexeme &) = default;
  Lexeme &operator=(Lexeme &&) noexcept = default;
  ~Lexeme() = default;

  TOKEN token() const;
  std::string literal() const;
  std::size_t linum() const;
  std::size_t token_start() const;

  friend std::ostream &operator<<(std::ostream &os, const Lexeme &lexeme);

private:
  TOKEN _token;
  std::string _literal;
  std::size_t _linum;
  std::size_t _token_start;
};

} // namespace gbpl0
