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

void Lexer::ingest_buffer() {
  // Open the file and check if it's opened successfully
  auto filestream = validate_and_open(_filename);
  if (!filestream.is_open()) {
    std::cerr << "error opening file\n";
    exit(1);
  }

  // Move to the end to get the file size
  filestream.seekg(0, std::ios::end);
  _filesize = filestream.tellg();

  // Move back to the beginning of the file
  filestream.seekg(0, std::ios::beg);

  // Resize the buffer to fit the file contents
  _buffer.resize(_filesize);

  // Read the file contents into the buffer
  filestream.read(_buffer.data(), _filesize);

  // Check if the read was successful
  if (!filestream) {
    std::cerr << "error reading source file into buffer\n";
    exit(1);
  }

  // Close the file stream
  filestream.close();

  std::cout << "Filename: " << _filename << "\nFile Size: " << _filesize
            << "\n";
}

const char Lexer::peek_next() const {
  return _pos + 1 < _filesize ? _buffer[_pos + 1] : '\0';
}

const char Lexer::peek() const {
  return _pos < _filesize ? _buffer[_pos] : '\0';
}

TOKEN Lexer::token() const { return _token; }

std::string Lexer::literal() const { return _literal; }

std::size_t Lexer::linum() const { return _linum; }

std::size_t Lexer::pos() const { return _pos; }

std::size_t Lexer::lpos() const { return _lpos; }

std::size_t Lexer::tstart() const { return _token_start; }

char Lexer::advance() {
  ++_pos;
  ++_lpos;
  return peek();
}

void Lexer::dump_lexeme() const {
  std::cout << "[" << _filename << "]" << linum() << ":" << tstart() << ": "
            << static_cast<char>(token()) << " '" << literal() << "'"
            << "\n";
}

void Lexer::match(const char rhs) {}

const void Lexer::parse_error(const std::string &err) const {
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

Lexeme Lexer::next_lexeme() {
  for (; _pos < _filesize;) {
    _literal = "";
    _token_start = _lpos;
    const auto c = advance();
    std::cout << _pos << "\n";

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
      std::cout << "LPAREN\n";
      return TOKEN::LPAREN;
    case ')':

      std::cout << "RPAREN\n";
      return TOKEN::RPAREN;
    case ':':
      if (peek_next() != '=') {
        parse_error("unexpected token");
        exit(1);
      }
      return TOKEN::ASSIGN;
    case '\0':
      return TOKEN::ENDFILE;
    default:
      parse_error("unexpected token");
      exit(1);
    }
  }

  return Lexeme(_token, _literal);
}

// void Lexer::parse() {
//  for (; _pos < _filesize; advance()) {
//     _token = next_lexeme();
//     switch (_token) {
//     case TOKEN::IDENT:
//     case TOKEN::NUMBER:
//     case TOKEN::CONST:
//     case TOKEN::VAR:
//     case TOKEN::PROCEDURE:
//     case TOKEN::CALL:
//     case TOKEN::BEGIN:
//     case TOKEN::END:
//     case TOKEN::IF:
//     case TOKEN::THEN:
//     case TOKEN::WHILE:
//     case TOKEN::DO:
//     case TOKEN::ODD:
//       dump_lexeme();
//       break;
//     case TOKEN::DOT:
//     case TOKEN::EQUAL:
//     case TOKEN::COMMA:
//     case TOKEN::SEMICOLON:
//     case TOKEN::HASH:
//     case TOKEN::LESSTHAN:
//     case TOKEN::GREATERTHAN:
//     case TOKEN::PLUS:
//     case TOKEN::MINUS:
//     case TOKEN::MULTIPLY:
//     case TOKEN::DIVIDE:
//     case TOKEN::LPAREN:
//     case TOKEN::RPAREN:
//       dump_lexeme();
//       break;
//     case TOKEN::ASSIGN:
//     case TOKEN::ENDFILE:
//       break;
//     case TOKEN::UNKNOWN:
//       std::cout << "unknown token found\n";
//       exit(1);
//     }
//   }
// }

} // namespace gbpl0
