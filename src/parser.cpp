#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>

namespace gbpl0 {

void Parser::next() { _lexemes.emplace_back(_lexer.next()); }

void Parser::run() {
  Lexeme lex{TOKEN::ENDFILE};
  /* do { */
  /*   lex = _lexer.next(); */
  /*   _lexemes.push_back(lex); */
  /*   _lexer.dump_lexeme(lex); */
  /* } while (lex.token() != TOKEN::ENDFILE); */
}

} // namespace gbpl0
