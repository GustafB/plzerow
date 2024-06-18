#pragma once

#include "token_type.hpp"
#include <cstddef>
#include <memory>
#include <variant>

namespace plzerow {

struct ASTNode;
struct Program;
struct Block;
struct Statement;
struct Expression;
struct Condition;
struct Assign;
struct Unary;
struct Binary;
struct Call;
struct Grouping;
struct Literal;
struct Logical;
struct Variable;

using NodeKind =
    std::variant<Program, Block, Statement, Expression, Condition, Assign,
                 Unary, Binary, Call, Grouping, Literal, Logical, Variable>;

using NodeHandle = std::unique_ptr<ASTNode>;

struct Program {
  Program();
};

struct Block {};
struct Statement {};
struct Expression {};
struct Condition {};
struct Assign {};
struct Unary {};
struct Binary {};
struct Call {};
struct Grouping {};
struct Literal {};
struct Logical {};
struct Variable {};

struct ASTNode {
public:
  ASTNode(TOKEN token_type, std::size_t linum, std::size_t column);
  template <typename Visitor> auto accept(Visitor &&visitor) const;

  NodeKind _kind;
  TOKEN _type;
  std::size_t _column;
  std::size_t _linum;
};

template <typename Visitor> auto ASTNode::accept(Visitor &&visitor) const {
  return std::visit(std::forward<Visitor>(visitor), *this);
}

// struct Unary : ASTNode {
// public:
//   Unary(TOKEN token_type, std::unique_ptr<ASTNode> rhs, std::size_t linum,
//         std::size_t column);

// private:
//   std::unique_ptr<ASTNode> _rhs;
// };

// struct Binary : ASTNode {
// public:
//   Binary(TOKEN token_type, std::unique_ptr<ASTNode> lhs,
//          std::unique_ptr<ASTNode> rhs, std::size_t linum, std::size_t
//          column);

// private:
//   std::unique_ptr<ASTNode> _lhs;
//   std::unique_ptr<ASTNode> _rhs;
// };

} // namespace plzerow
