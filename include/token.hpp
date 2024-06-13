#pragma once

#include "token_type.hpp"
#include <ostream>
#include <string>
#include <utility>

namespace plzerow {

class Token {
public:
  Token(TOKEN token, std::size_t linum, std::size_t token_start)
      : _token{token}, _literal{""}, _linum{linum}, _token_start{token_start} {}
  Token(TOKEN token, const std::string &literal, std::size_t linum,
        std::size_t token_start)
      : _token{token}, _literal{literal}, _linum{linum},
        _token_start{token_start} {}
  Token(TOKEN token, std::string &&literal, std::size_t linum,
        std::size_t token_start)
      : _token{token}, _literal{std::move(literal)}, _linum{linum},
        _token_start{token_start} {}

  Token(const Token &) = default;
  Token(Token &&) noexcept = default;
  Token &operator=(const Token &) = default;
  Token &operator=(Token &&) noexcept = default;
  ~Token() = default;

  TOKEN type() const;
  std::string literal() const;
  std::size_t linum() const;
  std::size_t token_start() const;

  friend std::ostream &operator<<(std::ostream &os, const Token &lexeme);

private:
  TOKEN _token;
  std::string _literal;
  std::size_t _linum;
  std::size_t _token_start;
};

} // namespace plzerow
