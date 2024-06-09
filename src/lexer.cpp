#include "lexer.hpp"
#include <cctype>
#include <iostream>
#include <string>

namespace gbpl0 {

void Lexer::read_file(const std::string &filename) {
  _buffer = _filehandler.read_file(filename);
  _filesize = _buffer.size();
  _filename = filename;
}

void Lexer::expect(TOKEN expected_token) {
  if (_token != expected_token) {
    parse_error("syntax error");
  }
  next();
}

char Lexer::peek_next() const {
  return _pos + 1 < _filesize ? _buffer[_pos + 1] : '\0';
}

char Lexer::peek() const { return _pos < _filesize ? _buffer[_pos] : '\0'; }

char Lexer::advance() {
  ++_pos;
  ++_lpos;
  return peek();
}

void Lexer::dump_lexeme(const Lexeme &lex) const {
  std::cout << _filename << ":" << _linum << ":" << _token_start << ": "
            << static_cast<char>(lex.token()) << " '" << lex.literal() << "'"
            << "\n";
}

void Lexer::parse_error(const std::string &err) const {
  std::cerr << err << ":" << _buffer[_pos] << " found at " << _linum << ":"
            << _pos;
}

void Lexer::parse_ident() {
  const auto pos = _pos;
  while (isalnum(peek()) || peek() == '_') {
    advance();
  }
  std::string identifier{_buffer.begin() + pos, _buffer.begin() + _pos};
  _literal = identifier;
  if (identifier == var)
    _token = TOKEN::VAR;
  else if (identifier == odd)
    _token = TOKEN::ODD;
  else if (identifier == constex)
    _token = TOKEN::CONST;
  else if (identifier == ifcond)
    _token = TOKEN::IF;
  else if (identifier == docond)
    _token = TOKEN::DO;
  else if (identifier == thencond)
    _token = TOKEN::THEN;
  else if (identifier == call)
    _token = TOKEN::CALL;
  else if (identifier == begin)
    _token = TOKEN::BEGIN;
  else if (identifier == wloop)
    _token = TOKEN::WHILE;
  else if (identifier == procedure)
    _token = TOKEN::PROCEDURE;
  else
    _token = TOKEN::IDENT;
  --_pos;
}

void Lexer::parse_number() {
  std::string number;
  while (std::isdigit(peek()) || peek() == '\'') {
    const auto digit = peek();
    if (peek() != '\'')
      number += digit;
    advance();
  }

  _literal = number;
  _token = TOKEN::NUMBER;
  --_pos;
}

void Lexer::parse_comment() {
  while (peek() != '}') {
    advance();
  }
  advance();
}

Lexeme Lexer::next() {
  while (_pos < _filesize) {
    _literal = "";
    _token = TOKEN::UNKNOWN;
    _token_start = _lpos;
    const auto c = advance();

    if (std::isalpha(c) || c == '_') {
      parse_ident();
      break;
    } else if (std::isdigit(c)) {
      parse_number();
      break;
    }

    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      break;
    case '\n':
      ++_linum;
      _lpos = 0;
      break;
    case '{':
      parse_comment();
      break;
    case '.':
      _token = TOKEN::DOT;
      return Lexeme(_token, ".");
    case '=':
      _token = TOKEN::EQUAL;
      return Lexeme(_token, "=");
    case ',':
      _token = TOKEN::COMMA;
      return Lexeme(_token, ",");
    case ';':
      _token = TOKEN::SEMICOLON;
      return Lexeme(_token, ";");
    case '#':
      _token = TOKEN::HASH;
      return Lexeme(_token, "#");
    case '<':
      _token = TOKEN::LESSTHAN;
      return Lexeme(_token, "<");
    case '>':
      _token = TOKEN::GREATERTHAN;
      return Lexeme(_token, ">");
    case '+':
      _token = TOKEN::PLUS;
      return Lexeme(_token, "+");
    case '-':
      _token = TOKEN::MINUS;
      return Lexeme(_token, "-");
    case '*':
      _token = TOKEN::MULTIPLY;
      return Lexeme(_token, "*");
    case '/':
      _token = TOKEN::DIVIDE;
      return Lexeme(_token, "/");
    case '(':
      _token = TOKEN::LPAREN;
      return Lexeme(_token, "(");
    case ')':
      _token = TOKEN::RPAREN;
      return Lexeme(_token, ")");
    case ':':
      if (peek_next() != '=') {
        parse_error("unexpected token");
        exit(1);
      }
      _token = TOKEN::ASSIGN;
      return Lexeme(_token, ":=");
    case '\0':
      _token = TOKEN::ENDFILE;
      return Lexeme(_token, "");
    default:
      parse_error("unexpected token");
      exit(1);
    }
  }

  return Lexeme(_token, _literal);
}

} // namespace gbpl0
