
#pragma once

#include <memory>
#include <plzerow/token_type.hpp>
#include <string>
#include <variant>
#include <vector>

namespace plzerow {

class ASTNode;

using Expression = std::unique_ptr<ASTNode>;
using ExprVector = std::vector<Expression>;

struct Grouping;
struct Equality;
struct Comparison;
struct Term;
struct Unary;
struct Binary;
struct Factor;
struct Primary;
struct Literal;

struct Grouping {
  Grouping(std::unique_ptr<ASTNode> expression)
      : _expression(std::move(expression)) {}
  Grouping(Grouping&&) = default;
  Grouping& operator=(Grouping&&) = default;
  std::unique_ptr<ASTNode> _expression;
};

struct Equality {
  Equality(std::unique_ptr<ASTNode> left, TOKEN op,
           std::unique_ptr<ASTNode> right)
      : _left(std::move(left)), _op(op), _right(std::move(right)) {}
  Equality(Equality&&) = default;
  Equality& operator=(Equality&&) = default;
  std::unique_ptr<ASTNode> _left;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Comparison {
  Comparison(std::unique_ptr<ASTNode> left, TOKEN op,
             std::unique_ptr<ASTNode> right)
      : _left(std::move(left)), _op(op), _right(std::move(right)) {}
  Comparison(Comparison&&) = default;
  Comparison& operator=(Comparison&&) = default;
  std::unique_ptr<ASTNode> _left;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Term {
  Term(std::unique_ptr<ASTNode> left, TOKEN op, std::unique_ptr<ASTNode> right)
      : _left(std::move(left)), _op(op), _right(std::move(right)) {}
  Term(Term&&) = default;
  Term& operator=(Term&&) = default;
  std::unique_ptr<ASTNode> _left;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Unary {
  Unary(TOKEN op, std::unique_ptr<ASTNode> right)
      : _op(op), _right(std::move(right)) {}
  Unary(Unary&&) = default;
  Unary& operator=(Unary&&) = default;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Binary {
  Binary(std::unique_ptr<ASTNode> left, TOKEN op,
         std::unique_ptr<ASTNode> right)
      : _left(std::move(left)), _op(op), _right(std::move(right)) {}
  Binary(Binary&&) = default;
  Binary& operator=(Binary&&) = default;
  std::unique_ptr<ASTNode> _left;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Factor {
  Factor(std::unique_ptr<ASTNode> left, TOKEN op,
         std::unique_ptr<ASTNode> right)
      : _left(std::move(left)), _op(op), _right(std::move(right)) {}
  Factor(Factor&&) = default;
  Factor& operator=(Factor&&) = default;
  std::unique_ptr<ASTNode> _left;
  TOKEN _op;
  std::unique_ptr<ASTNode> _right;
};

struct Primary {
  Primary(std::unique_ptr<ASTNode> literal) : _literal(std::move(literal)) {}
  Primary(Primary&&) = default;
  Primary& operator=(Primary&&) = default;
  std::unique_ptr<ASTNode> _literal;
};

struct Literal {
  Literal(std::string value) : _value(value), _type(TOKEN::STRING) {}
  Literal(bool value) : _value(value), _type(TOKEN::BOOL) {}
  Literal(std::int32_t value) : _value(value), _type(TOKEN::INTEGER) {}
  Literal(double value) : _value(value), _type(TOKEN::DOUBLE) {}

  Literal(Literal&&) = default;
  Literal& operator=(Literal&&) = default;

  bool as_bool() const;
  const std::string& as_string() const;
  std::int32_t as_int() const;
  double as_double() const;

  std::variant<std::int32_t, double, std::string, bool> _value;
  TOKEN _type;
};

class ASTNode {
 public:
  template <typename T>
  ASTNode(std::size_t linum, std::size_t column, T&& value);
  template <typename Visitor>
  auto accept(Visitor&& visitor) const;

  std::size_t _linum;
  std::size_t _column;
  std::variant<Grouping, Equality, Comparison, Term, Unary, Binary, Factor,
               Primary, Literal>
      _value;
};

template <typename T>
ASTNode::ASTNode(std::size_t linum, std::size_t column, T&& value)
    : _linum(linum), _column(column), _value(std::move(value)) {}

template <typename Visitor>
auto ASTNode::accept(Visitor&& visitor) const {
  return std::visit(std::forward<Visitor>(visitor), _value);
}

template <typename T, typename... Args>
std::unique_ptr<ASTNode> make_ast_node(std::size_t linum, std::size_t column,
                                       Args&&... args) {
  return std::make_unique<ASTNode>(linum, column,
                                   T{std::forward<Args>(args)...});
}

}  // namespace plzerow
