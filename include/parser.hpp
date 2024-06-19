#pragma once

#include "ast_nodes.hpp"
#include "token.hpp"
#include <functional>
#include <memory>
#include <sstream>
#include <vector>

namespace {

using Tokens = std::vector<plzerow::Token>;
using TokenIterator = Tokens::const_iterator;
using TokenGenerator = std::function<plzerow::Token()>;

} // namespace

namespace plzerow {

class Parser {
public:
  Parser();
  Parser(TokenGenerator &&next_token);
  std::unique_ptr<ASTNode> parse();

private:
  const Token &current() const;
  const Token &previous() const;
  void expect(TOKEN expected_token);
  void next();

  template <typename... Types> bool match(Types... types);
  bool match(std::initializer_list<TOKEN> types);

  std::unique_ptr<ASTNode> block();
  std::unique_ptr<ASTNode> statement();
  std::unique_ptr<ASTNode> expression();
  std::unique_ptr<ASTNode> condition();

  std::unique_ptr<ASTNode> term();
  std::unique_ptr<ASTNode> factor();
  std::unique_ptr<ASTNode> equality();
  std::unique_ptr<ASTNode> comparison();
  std::unique_ptr<ASTNode> unary();
  std::unique_ptr<ASTNode> primary();

  std::unique_ptr<ASTNode> make_constant();
  std::unique_ptr<ASTNode> make_var();
  std::unique_ptr<ASTNode> make_procedure();

  template <typename T, typename... Args>
  std::unique_ptr<ASTNode> make_node(Args &&...args);

  void parse_error(const std::string &err) const;

  std::function<Token()> _next_token;
  Token _current;
  Token _previous;
  std::unique_ptr<ASTNode> _program;
};

template <typename T, typename... Args>
std::unique_ptr<ASTNode> Parser::make_node(Args &&...args) {
  return make_ast_node<T>(current().linum(), current().token_start(),
                          T{std::forward<Args>(args)...});
}

template <typename... Types> bool Parser::match(Types... types) {
  return match({types...});
}

} // namespace plzerow
