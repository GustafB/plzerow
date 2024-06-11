#include "lexeme.hpp"
#include "token.hpp"
#include <ostream>

namespace plzerow {

TOKEN Lexeme::token() const { return _token; }

std::string Lexeme::literal() const { return _literal; }
std::size_t Lexeme::linum() const { return _linum; }
std::size_t Lexeme::token_start() const { return _token_start; }

std::ostream &operator<<(std::ostream &os, const Lexeme &lexeme) {
  os << lexeme.linum() << ":" << lexeme.token_start() << " Token='"
     << static_cast<char>(lexeme._token) << "' Literal='" << lexeme._literal
     << "'";
  return os;
}
} // namespace plzerow
