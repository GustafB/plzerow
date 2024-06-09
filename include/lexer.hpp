#pragma once

#include "filehandler.hpp"
#include "lexeme.hpp"
#include "token.hpp"
#include <string>
#include <vector>

namespace gbpl0 {

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
  Lexer(Lexer &&) noexcept = default;
  Lexer &operator=(Lexer &&) noexcept = default;
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  ~Lexer() = default;

  Lexeme next();
  void expect(TOKEN token);
  void read_file(const std::string &filename);

  // debugging
  void dump_lexeme(const Lexeme &lex) const;

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
