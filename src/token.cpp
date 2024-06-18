#include "token.hpp"
#include "token_type.hpp"
#include <ostream>

namespace plzerow {

TOKEN Token::type() const { return _token; }
const std::string &Token::literal() const { return _literal; }
std::size_t Token::linum() const { return _linum; }
std::size_t Token::token_start() const { return _token_start; }

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.linum() << ":" << token.token_start() << " Token='"
     << static_cast<char>(token._token) << "' Literal='" << token._literal
     << "'";
  return os;
}
} // namespace plzerow
