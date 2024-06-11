#include "parser.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <iostream>
#include <sstream>

namespace plzerow {

void Parser::parse_error(const std::string &err) const {
  std::cerr << "ERROR:" << current().linum() << ":" << current().token_start()
            << ": " << err << "\n";
}

void Parser::next() {
  _lexemes.emplace_back(_lexer.next());
  std::cout << current() << "\n";
}

const Lexeme &Parser::current() const { return _lexemes.back(); }

void Parser::read_tokens() {
  Lexeme lex{TOKEN::ENDFILE, 0, 1};
  do {
    next();

    std::cout << current() << "\n";

  } while (!_lexer.at_end());
}

void Parser::expect(TOKEN expected_token) {
  if (current().token() != expected_token) {
    std::stringstream err;
    err << "syntax error: expected token '" << static_cast<char>(expected_token)
        << "' but found '" << static_cast<char>(current().token()) << "'\n";

    _lexer.parse_error(err.str());
  }
  next();
}

void Parser::run() {
  next();
  block();
  expect(TOKEN::DOT);
}

void Parser::block() {
  if (current().token() == TOKEN::CONST) {
    std::cout << "parsing const block\n";
    expect(TOKEN::CONST);
    expect(TOKEN::IDENT);
    expect(TOKEN::EQUAL);
    expect(TOKEN::NUMBER);

    while (current().token() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      expect(TOKEN::IDENT);
      expect(TOKEN::EQUAL);
      expect(TOKEN::NUMBER);
    }

    expect(TOKEN::SEMICOLON);
  }

  if (current().token() == TOKEN::VAR) {
    std::cout << "parsing var block\n";
    expect(TOKEN::VAR);
    expect(TOKEN::IDENT);

    while (current().token() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      expect(TOKEN::IDENT);
    }

    expect(TOKEN::SEMICOLON);
  }

  while (current().token() == TOKEN::PROCEDURE) {
    std::cout << "parsing procedure block\n";
    expect(TOKEN::PROCEDURE);
    expect(TOKEN::IDENT);
    expect(TOKEN::SEMICOLON);

    block();

    expect(TOKEN::SEMICOLON);
  }

  statement();
}

void Parser::statement() {
  std::cout << "parsing statement\n";
  switch (current().token()) {
  case TOKEN::IDENT:
    expect(TOKEN::IDENT);
    expect(TOKEN::ASSIGN);
    expression();
    break;
  case TOKEN::BEGIN:
    expect(TOKEN::BEGIN);
    statement();
    while (current().token() == TOKEN::SEMICOLON) {
      expect(TOKEN::SEMICOLON);
      statement();
    }
    expect(TOKEN::END);
    break;
  case TOKEN::IF:
    expect(TOKEN::IF);
    condition();
    expect(TOKEN::THEN);
    statement();
    break;
  case TOKEN::WHILE:
    expect(TOKEN::WHILE);
    condition();
    expect(TOKEN::DO);
    statement();
  }
}

void Parser::condition() {
  std::cout << "parsing condition\n";
  if (current().token() == TOKEN::ODD) {
    expect(TOKEN::ODD);
    expression();
  } else {

    expression();

    switch (current().token()) {
    case TOKEN::EQUAL:
    case TOKEN::HASH:
    case TOKEN::LESSTHAN:
    case TOKEN::GREATERTHAN:
      next();
      break;
    default:
      std::cerr << "invalid conditional\n";
      next();
    }
    expression();
  }
}

void Parser::expression() {
  std::cout << "parsing expression\n";
  if (current().token() == TOKEN::PLUS || current().token() == TOKEN::MINUS) {
    next();
  }

  term();

  while (current().token() == TOKEN::PLUS ||
         current().token() == TOKEN::MINUS) {
    next();
    term();
  }
}

void Parser::factor() {
  std::cout << "parsing factor\n";
  switch (current().token()) {
  case TOKEN::IDENT:
  case TOKEN::NUMBER:
    next();
    break;
  case TOKEN::LPAREN:
    expect(TOKEN::LPAREN);
    expression();
    expect(TOKEN::RPAREN);
    break;
  }
}

void Parser::term() {
  std::cout << "parsing term\n";
  factor();

  while (current().token() == TOKEN::MULTIPLY ||
         current().token() == TOKEN::DIVIDE) {
    next();
    factor();
  }
}

} // namespace plzerow
