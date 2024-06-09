#include "lexeme.hpp"
#include "token.hpp"

namespace gbpl0 {

TOKEN Lexeme::token() const { return _token; }

std::string Lexeme::literal() const { return _literal; }

} // namespace gbpl0
