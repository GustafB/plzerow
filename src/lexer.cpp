#include "lexer.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

namespace plzerow {

void Lexer::read_file(const std::string &filename) {
  _buffer = _filehandler.read_file(filename);
  _filesize = _buffer.size();
  _filename = filename;
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

void Lexer::dump_lexeme(const Lexeme &lex) const {
  std::cout << _filename << ":" << _linum << ":" << _token_line_pos << ": "
            << static_cast<char>(lex.token()) << " '" << lex.literal() << "'"
            << "\n";
}

void Lexer::parse_error(const std::string &err) const {
  std::cerr << _filename << ":" << _linum << ":" << _token_line_pos << ": "
            << " ERROR: " << err << "\n";
}

Lexeme Lexer::parse_ident() {
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

  return Lexeme(_token, _literal, _linum, _token_line_pos);
}

Lexeme Lexer::parse_number() {
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
  return Lexeme(_token, _literal, _linum, _token_line_pos);
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

Lexeme Lexer::next() {
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
    return Lexeme(_token, ".", _linum, _token_line_pos);
  case '=':
    _token = TOKEN::EQUAL;
    return Lexeme(_token, "=", _linum, _token_line_pos);
  case ',':
    _token = TOKEN::COMMA;
    return Lexeme(_token, ",", _linum, _token_line_pos);
  case ';':
    _token = TOKEN::SEMICOLON;
    return Lexeme(_token, ";", _linum, _token_line_pos);
  case '#':
    _token = TOKEN::HASH;
    return Lexeme(_token, "#", _linum, _token_line_pos);
  case '<':
    _token = TOKEN::LESSTHAN;
    return Lexeme(_token, "<", _linum, _token_line_pos);
  case '>':
    _token = TOKEN::GREATERTHAN;
    return Lexeme(_token, ">", _linum, _token_line_pos);
  case '+':
    _token = TOKEN::PLUS;
    return Lexeme(_token, "+", _linum, _token_line_pos);
  case '-':
    _token = TOKEN::MINUS;
    return Lexeme(_token, "-", _linum, _token_line_pos);
  case '*':
    _token = TOKEN::MULTIPLY;
    return Lexeme(_token, "*", _linum, _token_line_pos);
  case '/':
    _token = TOKEN::DIVIDE;
    return Lexeme(_token, "/", _linum, _token_line_pos);
  case '(':
    _token = TOKEN::LPAREN;
    return Lexeme(_token, "(", _linum, _token_line_pos);
  case ')':
    _token = TOKEN::RPAREN;
    return Lexeme(_token, ")", _linum, _token_line_pos);
  case ':':
    if (peek() != '=') {
      parse_error("unexpected token");
      exit(1);
    }
    _token = TOKEN::ASSIGN;
    advance();
    return Lexeme(_token, ":=", _linum, _token_line_pos);
  case '\0':
    _token = TOKEN::ENDFILE;
    return Lexeme(_token, "", _linum, _token_line_pos);
  default:
    parse_error("unexpected token");
    exit(1);
  }

  return Lexeme(TOKEN::ENDFILE, "\0", _linum, _token_line_pos);
}

} // namespace plzerow
