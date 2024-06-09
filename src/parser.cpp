#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>

namespace gbpl0 {

void Parser::next() { _lexemes.emplace_back(_lexer.next()); }

void Parser::run() {
  Lexeme lex{TOKEN::ENDFILE};
  do {
    lex = _lexer.next();
    _lexemes.push_back(lex);
    std::cout << "lex=" << static_cast<char>(lex._token) << ": " << lex._literal
              << "\n";
  } while (lex._token != TOKEN::ENDFILE);
}

} // namespace gbpl0
