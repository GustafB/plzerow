#include "parser.hpp"
#include "token_type.hpp"
#include <iostream>
#include <sstream>

namespace plzerow {

Parser::Parser()
    : _next_token(
          []() { return plzerow::Token(plzerow::TOKEN::ENDFILE, 0, 0); }),
      _current{_next_token()}, _previous{TOKEN::ENDFILE, 0, 0} {}

Parser::Parser(std::function<Token()> next_token)
    : _next_token(next_token), _current{_next_token()},
      _previous{TOKEN::ENDFILE, 0, 0} {}

void Parser::parse_error(const std::string &err) const {
  std::cerr << "[PARSE_ERROR] [" << current().linum() << ":"
            << current().token_start() << "] " << err << "\n";
}

void Parser::next() {
  _previous = _current;
  _current = _next_token();
}

const Token &Parser::current() const { return _current; }

void Parser::expect(TOKEN expected_token) {
  if (current().type() != expected_token) {
    std::stringstream err;

    if (current().type() == TOKEN::ENDFILE) {
      err << "end of file reached without terminator '.'";
      parse_error(err.str());
      return;
    }

    err << "syntax error: expected token '" << static_cast<char>(expected_token)
        << "' but found '" << static_cast<char>(current().type()) << "'\n";
    parse_error(err.str());
    return;
  }
  next();
}

void Parser::parse() {
  block();
  expect(TOKEN::DOT);
}

void Parser::block() {
  if (current().type() == TOKEN::CONST) {
    expect(TOKEN::CONST);
    expect(TOKEN::IDENT);
    expect(TOKEN::EQUAL);
    expect(TOKEN::NUMBER);

    while (current().type() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      expect(TOKEN::IDENT);
      expect(TOKEN::EQUAL);
      expect(TOKEN::NUMBER);
    }

    expect(TOKEN::SEMICOLON);
  }

  if (current().type() == TOKEN::VAR) {
    expect(TOKEN::VAR);
    expect(TOKEN::IDENT);

    while (current().type() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      expect(TOKEN::IDENT);
    }

    expect(TOKEN::SEMICOLON);
  }

  while (current().type() == TOKEN::PROCEDURE) {
    expect(TOKEN::PROCEDURE);
    expect(TOKEN::IDENT);
    expect(TOKEN::SEMICOLON);

    block();

    expect(TOKEN::SEMICOLON);
  }

  statement();
}

void Parser::statement() {
  switch (current().type()) {
  case TOKEN::IDENT:
    expect(TOKEN::IDENT);
    expect(TOKEN::ASSIGN);
    expression();
    break;
  case TOKEN::BEGIN:
    expect(TOKEN::BEGIN);
    statement();
    while (current().type() == TOKEN::SEMICOLON) {
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
  if (current().type() == TOKEN::ODD) {
    expect(TOKEN::ODD);
    expression();
  } else {

    expression();

    switch (current().type()) {
    case TOKEN::EQUAL:
    case TOKEN::HASH:
    case TOKEN::LESSTHAN:
    case TOKEN::GREATERTHAN:
      next();
      break;
    default:
      next();
    }
    expression();
  }
}

void Parser::expression() {
  if (current().type() == TOKEN::PLUS || current().type() == TOKEN::MINUS) {
    next();
  }

  term();

  while (current().type() == TOKEN::PLUS || current().type() == TOKEN::MINUS) {
    next();
    term();
  }
}

void Parser::factor() {
  switch (current().type()) {
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
  factor();

  while (current().type() == TOKEN::MULTIPLY ||
         current().type() == TOKEN::DIVIDE) {
    next();
    factor();
  }
}

} // namespace plzerow
