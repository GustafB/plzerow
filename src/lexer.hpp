#pragma once

#include "filehandler.hpp"
#include <string>
#include <vector>

namespace gbpl0 {

enum class TOKEN {
  IDENT = 'I',
  NUMBER = 'N',
  CONST = 'C',
  VAR = 'V',
  PROCEDURE = 'P',
  CALL = 'c',
  BEGIN = 'B',
  END = 'E',
  IF = 'i',
  THEN = 'T',
  WHILE = 'W',
  DO = 'D',
  ODD = 'O',
  DOT = '.',
  EQUAL = '=',
  COMMA = ',',
  SEMICOLON = ';',
  ASSIGN = ':',
  HASH = '#',
  LESSTHAN = '<',
  GREATERTHAN = '>',
  PLUS = '+',
  MINUS = '-',
  MULTIPLY = '*',
  DIVIDE = '/',
  LPAREN = '(',
  RPAREN = ')',
  ENDFILE = '\0',
  UNKNOWN,
};

constexpr char var[] = "var";
constexpr char odd[] = "odd";
constexpr char constex[] = "const";
constexpr char ifcond[] = "ifcond";
constexpr char docond[] = "docond";
constexpr char thencond[] = "thencond";
constexpr char elsecond[] = "elsecond";
constexpr char call[] = "call";
constexpr char begin[] = "begin";
constexpr char floop[] = "for";
constexpr char wloop[] = "while";
constexpr char procedure[] = "procedure";

struct Lexeme {
  Lexeme(TOKEN token) : _token{token}, _literal{""} {};
  Lexeme(TOKEN token, const std::string literal)
      : _token{token}, _literal{literal} {};

  TOKEN _token;
  std::string _literal;
};

class Lexer {
public:
  Lexer() = default;
  Lexer(Lexer &&) noexcept = default;
  Lexer &operator=(Lexer &&) noexcept = default;
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;

  Lexeme next();
  void expect(TOKEN token);
  void read_file(const std::string &filename);

private:
  // parse expressions
  void parse_comment();
  void parse_ident();
  void parse_number();

  // helpers
  char peek() const;
  char peek_next() const;
  void parse_error(const std::string &err) const;
  void match(const char rhs);
  char advance();
  void dump_lexeme() const;

  // active state
  std::vector<char> _buffer;
  std::string _literal;
  TOKEN _token;

  // position tracking
  std::size_t _linum = 0;
  std::size_t _lpos = 1;
  std::size_t _token_start = 0;
  std::size_t _pos = 0;
  std::size_t _filesize = 0;
  std::string _filename;

  // file reader
  FileHandler _filehandler{".pl0"};
};

} // namespace gbpl0
