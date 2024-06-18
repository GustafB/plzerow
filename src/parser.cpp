#include "parser.hpp"
#include "ast_nodes.hpp"
#include "token_type.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

namespace plzerow {

Parser::Parser()
    : _next_token{[]() {
        return plzerow::Token(plzerow::TOKEN::ENDFILE, 0, 0);
      }},
      _current{_next_token()}, _previous{TOKEN::ENDFILE, 0, 0} {}

Parser::Parser(std::function<Token()> &&next_token)
    : _next_token(std::forward<TokenGenerator>(next_token)),
      _current{_next_token()}, _previous{TOKEN::ENDFILE, 0, 0} {}

void Parser::parse_error(const std::string &err) const {
  std::cerr << "[PARSE_ERROR] [" << current().linum() << ":"
            << current().token_start() << "] " << err << "\n";
}

void Parser::next() {
  std::cout << _current << "\n";
  _previous = _current;
  _current = _next_token();
}

const Token &Parser::current() const { return _current; }
const Token &Parser::previous() const { return _previous; }

void Parser::expect(TOKEN expected_token) {
  if (current().type() != expected_token) {
    std::stringstream err;

    if (current().type() == TOKEN::ENDFILE) {
      err << "end of file reached without terminator '.'";
      parse_error(err.str());
      return;
    }

    err << "syntax error: expected token '" << static_cast<char>(expected_token)
        << "' but found '" << static_cast<char>(current().type()) << "'\n";
    parse_error(err.str());
    return;
  }
  next();
}

std::unique_ptr<ASTNode> Parser::make_constant() {
  const auto ident = current().literal();
  expect(TOKEN::IDENT);
  expect(TOKEN::EQUAL);
  expect(TOKEN::NUMBER);
  auto number_literal = previous().literal();
  auto number = std::atoi(number_literal.c_str());
  return make_ast_node<ConstDecl>(previous().linum(), previous().token_start(),
                                  ident, number);
}

std::unique_ptr<ASTNode> Parser::make_var() {
  auto var_decl = make_ast_node<VarDecl>(
      current().linum(), current().token_start(), current().literal());
  expect(TOKEN::IDENT);
  return var_decl;
}

std::unique_ptr<ASTNode> Parser::make_procedure() {
  const auto linum = current().linum();
  const auto column = current().token_start();

  expect(TOKEN::PROCEDURE);
  const auto name = current().literal();
  expect(TOKEN::IDENT);
  expect(TOKEN::SEMICOLON);

  auto blk = block();

  expect(TOKEN::SEMICOLON);
  return make_ast_node<Procedure>(linum, column, name, std::move(blk));
}

std::unique_ptr<ASTNode> Parser::parse() {
  auto blk = block();
  auto program = make_ast_node<Program>(0, 0, std::move(blk));
  expect(TOKEN::DOT);
  return program;
}

std::unique_ptr<ASTNode> Parser::block() {
  std::vector<std::unique_ptr<ASTNode>> const_decls;
  std::vector<std::unique_ptr<ASTNode>> var_decls;
  std::vector<std::unique_ptr<ASTNode>> procedures;
  const auto linum = current().linum();
  const auto column = current().token_start();

  if (current().type() == TOKEN::CONST) {
    expect(TOKEN::CONST);
    const_decls.push_back(make_constant());

    while (current().type() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      const_decls.push_back(make_constant());
    }

    expect(TOKEN::SEMICOLON);
  }

  if (current().type() == TOKEN::VAR) {
    expect(TOKEN::VAR);
    var_decls.push_back(make_var());

    while (current().type() == TOKEN::COMMA) {
      expect(TOKEN::COMMA);
      var_decls.push_back(make_var());
    }

    expect(TOKEN::SEMICOLON);
  }

  while (current().type() == TOKEN::PROCEDURE) {
    procedures.push_back(make_procedure());
  }

  auto stmt = statement();
  return make_ast_node<Block>(linum, column, std::move(const_decls),
                              std::move(var_decls), std::move(procedures),
                              std::move(stmt));
}

std::unique_ptr<ASTNode> Parser::statement() {
  switch (current().type()) {
  case TOKEN::IDENT: {
    auto name = current().literal();
    expect(TOKEN::IDENT);
    expect(TOKEN::ASSIGN);
    auto expr = expression();
    return make_ast_node<Assignment>(
        previous().linum(), previous().token_start(), name, std::move(expr));
  }
  case TOKEN::BEGIN: {
    std::vector<std::unique_ptr<ASTNode>> stmts;
    expect(TOKEN::BEGIN);
    stmts.push_back(statement());
    while (current().type() == TOKEN::SEMICOLON) {
      expect(TOKEN::SEMICOLON);
      stmts.push_back(statement());
    }
    expect(TOKEN::END);
    return make_ast_node<Begin>(previous().linum(), previous().token_start(),
                                nullptr, std::move(stmts));
  }
  case TOKEN::IF: {
    expect(TOKEN::IF);
    auto cond = condition();
    expect(TOKEN::THEN);
    auto stmt = statement();
    return make_ast_node<If>(previous().linum(), previous().token_start(),
                             std::move(cond), std::move(stmt));
  }
  case TOKEN::WHILE: {
    expect(TOKEN::WHILE);
    auto cond = condition();
    expect(TOKEN::DO);
    auto stmt = statement();
    return make_ast_node<While>(previous().linum(), previous().token_start(),
                                std::move(cond), std::move(stmt));
  }
  default:
    return nullptr;
  }
}

std::unique_ptr<ASTNode> Parser::condition() {
  if (current().type() == TOKEN::ODD) {
    expect(TOKEN::ODD);
    auto expr = expression();
    return make_ast_node<OddCondition>(
        previous().linum(), previous().token_start(), std::move(expr));
  } else {
    auto left = expression();
    auto op = current().type();
    switch (op) {
    case TOKEN::EQUAL:
    case TOKEN::HASH:
    case TOKEN::LESSTHAN:
    case TOKEN::GREATERTHAN:
      next();
      break;
    default:
      next();
    }
    auto right = expression();
    return make_ast_node<Condition>(previous().linum(),
                                    previous().token_start(), op,
                                    std::move(left), std::move(right));
  }
}

std::unique_ptr<ASTNode> Parser::expression() {
  std::vector<std::pair<TOKEN, std::unique_ptr<ASTNode>>> terms;
  TOKEN op = current().type();
  if (op == TOKEN::PLUS || op == TOKEN::MINUS) {
    next();
  }
  auto term_node = term();
  while (current().type() == TOKEN::PLUS || current().type() == TOKEN::MINUS) {
    TOKEN current_op = current().type();
    next();
    terms.push_back({current_op, term()});
  }
  return make_ast_node<Expression>(previous().linum(), previous().token_start(),
                                   op, std::move(term_node), std::move(terms));
}

std::unique_ptr<ASTNode> Parser::factor() {
  TOKEN factor_op = previous().type();
  switch (current().type()) {
  case TOKEN::IDENT: {
    auto value = make_node<Primary>(current().literal());
    next();
    return make_node<Factor>(factor_op, std::move(value));
  }
  case TOKEN::NUMBER: {
    auto value = make_node<Primary>(current().literal());
    next();
    return make_node<Factor>(factor_op, std::move(value));
  }
  case TOKEN::LPAREN: {
    expect(TOKEN::LPAREN);
    auto expr = expression();
    expect(TOKEN::RPAREN);
    return make_node<Factor>(factor_op, std::move(expr));
  }
  }
  return nullptr;
}

std::unique_ptr<ASTNode> Parser::term() {
  std::vector<std::pair<TOKEN, std::unique_ptr<ASTNode>>> factors;
  auto fact = factor();
  TOKEN initial_token = current().type();
  while (current().type() == TOKEN::MULTIPLY ||
         current().type() == TOKEN::DIVIDE) {
    TOKEN current_token = current().type();
    next();
    factors.push_back({current_token, factor()});
  }
  return make_ast_node<Term>(previous().linum(), previous().token_start(),
                             initial_token, std::move(fact),
                             std::move(factors));
}

} // namespace plzerow
