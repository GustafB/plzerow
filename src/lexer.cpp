#include "lexer.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

namespace gbpl0 {

std::ifstream Lexer::validate_and_open(const std::string &filename) {
  const std::string file_extension = ".pl0";
  if (filename.length() >= file_extension.size()) {
    if (filename.rfind(file_extension) !=
        (filename.length() - file_extension.length())) {
      std::cerr << "file is not of the right type\n";
      exit(1);
    }
  }

  auto filestream = std::ifstream(filename, std::ios::binary | std::ios::ate);

  if (!filestream) {
    std::cerr << "unable to open filename: " << filename << "\n";
    exit(1);
  }

  return filestream;
}

void Lexer::ingest_buffer(const std::string &filename) {
  auto filestream = validate_and_open(filename);
  _filesize = filestream.tellg();
  filestream.seekg(0, std::ios::beg);
  _buffer.reserve(_filesize);
  filestream.read(_buffer.data(), _filesize);
  if (filestream.bad()) {
    std::cerr << "error reading source file into buffer\n";
    exit(1);
  }

  std::cout << "Filename: " << filename << "\nFile Size: " << _filesize << "\n";
}

const char Lexer::peek_next() const {
  return _pos + 1 < _filesize ? _buffer[_pos + 1] : '\0';
}

const char Lexer::peek() const {
  /*std::cout << _buffer[_pos] << "\n";*/
  return _pos < _filesize ? _buffer[_pos] : '\0';
}

std::string Lexer::token() const { return _token; }
std::size_t Lexer::linum() const { return _linum; }
std::size_t Lexer::pos() const { return _pos; }
std::size_t Lexer::lpos() const { return _lpos; }
std::size_t Lexer::tstart() const { return _token_start; }
void Lexer::advance() {
  ++_pos;
  ++_lpos;
}

const void Lexer::parse_error(const std::string &err) const {
  std::cerr << err << ":" << _buffer[_pos] << " found at " << _linum << ":"
            << _pos;
}

TOKEN Lexer::parse_ident() {
  const auto pos = _pos;
  while (isalnum(peek()) || peek() == '_') {
    advance();
  }
  std::string identifier{_buffer.begin() + pos, _buffer.begin() + _pos};
  _token = identifier;
  if (identifier == var)
    return TOKEN::VAR;
  else if (identifier == odd)
    return TOKEN::ODD;
  else if (identifier == constex)
    return TOKEN::CONST;
  else if (identifier == ifcond)
    return TOKEN::IF;
  else if (identifier == docond)
    return TOKEN::DO;
  else if (identifier == thencond)
    return TOKEN::THEN;
  else if (identifier == call)
    return TOKEN::CALL;
  else if (identifier == begin)
    return TOKEN::BEGIN;
  else if (identifier == wloop)
    return TOKEN::WHILE;
  else if (identifier == procedure)
    return TOKEN::PROCEDURE;
  else
    return TOKEN::IDENT;
}

TOKEN Lexer::parse_number() {
  std::string number;
  while (std::isdigit(peek()) || peek() == '\'') {
    if (peek() != '\'')
      number += peek();
    advance();
  }

  _token = number;

  return TOKEN::NUMBER;
}

void Lexer::parse_comment() {
  while (peek() != '}') {
    advance();
  }
  advance();
}

TOKEN Lexer::next_token() {
  for (; _pos < _filesize; advance()) {
    _token_start = _lpos;
    const auto c = peek();
    if (std::isalpha(c) || c == '_') {
      return parse_ident();
    }

    if (std::isdigit(c)) {
      return parse_number();
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
      return TOKEN::DOT;
    case '=':
      return TOKEN::EQUAL;
    case ',':
      return TOKEN::COMMA;
    case ';':
      return TOKEN::SEMICOLON;
    case '#':
      return TOKEN::HASH;
    case '<':
      return TOKEN::LESSTHAN;
    case '>':
      return TOKEN::GREATERTHAN;
    case '+':
      return TOKEN::PLUS;
    case '-':
      return TOKEN::MINUS;
    case '*':
      return TOKEN::MULTIPLY;
    case '/':
      return TOKEN::DIVIDE;
    case '(':
      return TOKEN::LPAREN;
    case ')':
      return TOKEN::RPAREN;
    case ':':
      if (peek_next() != '=')
        parse_error("unexpected token");
      exit(1);
    case '\0':
      return TOKEN::ENDFILE;
    default:
      parse_error("unexpected token");
      exit(1);
    }
  }
  return TOKEN::ENDFILE;
}

} // namespace gbpl0
