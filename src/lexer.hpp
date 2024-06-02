#pragma once
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

class Lexer {
public:
  Lexer() = default;
  void ingest_buffer(const std::string &filename);
  TOKEN next_token();

  // getters
  std::string token() const;
  std::size_t linum() const;
  std::size_t pos() const;
  std::size_t lpos() const;
  std::size_t tstart() const;

private:
  std::ifstream validate_and_open(const std::string &filename);

  // parse expressions
  void parse_comment();
  TOKEN parse_ident();
  TOKEN parse_number();

  // helpers
  const char peek() const;
  const char peek_next() const;
  const void parse_error(const std::string &err) const;
  void advance();

  std::vector<char> _buffer;
  std::string _token;
  std::size_t _linum;
  std::size_t _lpos = 1;
  std::size_t _token_start;
  std::size_t _pos;
  std::size_t _filesize;
};
} // namespace gbpl0
