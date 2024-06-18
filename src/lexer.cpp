#include "lexer.hpp"
#include "fmt/core.h"
#include "token.hpp"
#include "token_type.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

namespace plzerow {

Lexer::Lexer(std::vector<char> &&source)
    : _buffer{std::forward<std::vector<char>>(source)},
      _filesize{_buffer.size()}, _filename{"repl"} {}

Lexer::Lexer(const std::string &filename, std::vector<char> &&source)
    : _buffer{std::forward<std::vector<char>>(source)},
      _filesize{_buffer.size()}, _filename{filename} {}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  Token current{TOKEN::ENDFILE, 0, 1};

  do {
    tokens.push_back(next());
  } while (!at_end());

  return tokens;
}

char Lexer::peek_next() const {
  return _pos + 1 < _filesize ? _buffer[_pos + 1] : '\0';
}

char Lexer::peek() const { return _pos < _filesize ? _buffer[_pos] : '\0'; }

bool Lexer::at_end() const { return _pos >= _filesize; }

char Lexer::advance() {
  auto c = peek();
  ++_pos;
  ++_lpos;
  return c;
}

void Lexer::dump_lexeme(const Token &lex) const {
  std::cout << _filename << ":" << _linum << ":" << _token_line_pos << ": "
            << static_cast<char>(lex.type()) << " '" << lex.literal() << "'"
            << "\n";
}

void Lexer::parse_error(const std::string &err) const {
  std::cerr << "[LEXICAL_ERROR] [" << _filename << ":" << _linum << ":"
            << _token_line_pos << "] " << err << "\n";
}

Token Lexer::parse_ident() {
  const auto pos = _token_start;
  while (isalnum(peek()) || peek() == '_') {
    advance();
  }

  std::string identifier{_buffer.begin() + pos, _buffer.begin() + _pos};
  _literal = identifier;

  static const std::unordered_map<std::string, TOKEN> keywords = {
      {kw_var, TOKEN::VAR},
      {kw_odd, TOKEN::ODD},
      {kw_const, TOKEN::CONST},
      {kw_cond, TOKEN::IF},
      {kw_do, TOKEN::DO},
      {kw_then, TOKEN::THEN},
      {kw_call, TOKEN::CALL},
      {kw_begin, TOKEN::BEGIN},
      {kw_while, TOKEN::WHILE},
      {kw_end, TOKEN::END},
      {kw_procedure, TOKEN::PROCEDURE}};

  auto it = keywords.find(identifier);
  if (it != keywords.end()) {
    _token = it->second;
  } else {
    _token = TOKEN::IDENT;
  }

  return Token(_token, _literal, _linum, _token_line_pos);
}

Token Lexer::parse_number() {
  const auto pos = _token_start;
  std::string number;
  while (std::isdigit(peek()) || peek() == '\'') {
    advance();
  }

  std::remove_copy_if(_buffer.begin() + pos, _buffer.begin() + _pos,
                      std::back_inserter(number),
                      [](char c) { return c == '\''; });
  _literal = number;
  _token = TOKEN::NUMBER;
  return Token(_token, _literal, _linum, _token_line_pos);
}

void Lexer::parse_comment() {
  while (peek() != '}') {
    advance();
  }
  advance();
}

void Lexer::parse_whitespace() {
  for (;;) {
    const char c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    case '\n':
      ++_linum;
      _lpos = 0;
      advance();
      break;
    case '{':
      parse_comment();
      break;
    default:
      return;
    }
  }
}

Token Lexer::next() {
  parse_whitespace();

  _literal = "";
  _token = TOKEN::UNKNOWN;
  _token_line_pos = _lpos;
  _token_start = _pos;

  auto c = advance();

  if (std::isalpha(c) || c == '_') {
    return parse_ident();
  } else if (std::isdigit(c)) {
    return parse_number();
  }

  switch (c) {
  case '.':
    _token = TOKEN::DOT;
    break;
  case '=':
    _token = TOKEN::EQUAL;
    break;
  case ',':
    _token = TOKEN::COMMA;
    break;
  case ';':
    _token = TOKEN::SEMICOLON;
    break;
  case '#':
    _token = TOKEN::HASH;
    break;
  case '<':
    _token = TOKEN::LESSTHAN;
    break;
  case '>':
    _token = TOKEN::GREATERTHAN;
    break;
  case '+':
    _token = TOKEN::PLUS;
    break;
  case '-':
    _token = TOKEN::MINUS;
    break;
  case '*':
    _token = TOKEN::MULTIPLY;
    break;
  case '/':
    _token = TOKEN::DIVIDE;
    break;
  case '(':
    _token = TOKEN::LPAREN;
    break;
  case ')':
    _token = TOKEN::RPAREN;
    break;
  case ':':
    if (peek() != '=') {
      _token = TOKEN::ERROR;
      _literal = fmt::format("unexpected token {}", c);
    }
    _token = TOKEN::ASSIGN;
    advance();
    break;
  case '\0':
    _token = TOKEN::ENDFILE;
    break;
  default:
    _token = TOKEN::ERROR;
    _literal = fmt::format("unexpected token {}", c);
  }

  return Token(_token, _literal, _linum, _token_line_pos);
}

} // namespace plzerow
