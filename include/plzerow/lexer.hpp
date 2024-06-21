#pragma once

#include <plzerow/token.hpp>
#include <plzerow/token_type.hpp>
#include <string>
#include <vector>

namespace plzerow {

constexpr char kw_var[] = "var";
constexpr char kw_odd[] = "odd";
constexpr char kw_const[] = "const";
constexpr char kw_cond[] = "if";
constexpr char kw_do[] = "do";
constexpr char kw_then[] = "then";
constexpr char kw_else[] = "else";
constexpr char kw_call[] = "call";
constexpr char kw_begin[] = "begin";
constexpr char kw_for[] = "for";
constexpr char kw_while[] = "while";
constexpr char kw_procedure[] = "procedure";
constexpr char kw_end[] = "end";
constexpr char kw_print[] = "print";

class Lexer {
 public:
  Lexer() = default;
  Lexer(const std::string &filename, std::vector<char> &&source);
  Lexer(std::vector<char> &&source);

  Lexer(Lexer &&) noexcept = default;
  Lexer &operator=(Lexer &&) noexcept = default;
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  ~Lexer() = default;

  Token next();
  std::vector<Token> tokenize();

  // debugging
  void dump_lexeme(const Token &lex) const;
  void parse_error(const std::string &err) const;
  bool at_end() const;

 private:
  // parse expressions
  void parse_whitespace();
  void parse_comment();
  Token parse_ident();
  Token parse_number();
  Token parse_string();

  // helpers
  char peek() const;
  char peek_next() const;
  void match(const char rhs);
  char advance();

  // active state
  std::vector<char> _buffer;
  std::string _literal;
  TOKEN _token;

  // position tracking
  std::size_t _linum = 1;
  std::size_t _lpos = 1;
  std::size_t _token_line_pos = 0;
  std::size_t _token_start = 0;
  std::size_t _pos = 0;
  std::size_t _filesize = 0;
  std::string _filename;
};

}  // namespace plzerow
