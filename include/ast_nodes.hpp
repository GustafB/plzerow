
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "token_type.hpp"

namespace plzerow {

class ASTNode;

using ExprContainer = std::vector<std::pair<TOKEN, std::unique_ptr<ASTNode>>>;
using NodeContainer = std::vector<std::unique_ptr<ASTNode>>;

struct Block;
struct ConstDecl;
struct VarDecl;
struct Procedure;
struct Statement;
struct Assignment;
struct Call;
struct Begin;
struct If;
struct While;
struct Condition;
struct OddCondition;
struct Comparison;
struct Expression;
struct Term;
struct Factor;
struct Program;

struct Block {
    Block(NodeContainer constDecls, NodeContainer varDecls, NodeContainer procedures, std::unique_ptr<ASTNode> statement) : _constDecls(std::move(constDecls)), _varDecls(std::move(varDecls)), _procedures(std::move(procedures)), _statement(std::move(statement)) {}
    Block(Block&&) = default;
    Block& operator=(Block&&) = default;
    NodeContainer _constDecls;
    NodeContainer _varDecls;
    NodeContainer _procedures;
    std::unique_ptr<ASTNode> _statement;
};

struct ConstDecl {
    ConstDecl(std::string name, int value) : _name(name), _value(value) {}
    ConstDecl(ConstDecl&&) = default;
    ConstDecl& operator=(ConstDecl&&) = default;
    std::string _name;
    int _value;
};

struct VarDecl {
    VarDecl(std::string name) : _name(name) {}
    VarDecl(VarDecl&&) = default;
    VarDecl& operator=(VarDecl&&) = default;
    std::string _name;
};

struct Procedure {
    Procedure(std::string name, std::unique_ptr<ASTNode> block) : _name(name), _block(std::move(block)) {}
    Procedure(Procedure&&) = default;
    Procedure& operator=(Procedure&&) = default;
    std::string _name;
    std::unique_ptr<ASTNode> _block;
};

struct Statement {
    Statement(std::unique_ptr<ASTNode> statement) : _statement(std::move(statement)) {}
    Statement(Statement&&) = default;
    Statement& operator=(Statement&&) = default;
    std::unique_ptr<ASTNode> _statement;
};

struct Assignment {
    Assignment(std::string name, std::unique_ptr<ASTNode> expression) : _name(name), _expression(std::move(expression)) {}
    Assignment(Assignment&&) = default;
    Assignment& operator=(Assignment&&) = default;
    std::string _name;
    std::unique_ptr<ASTNode> _expression;
};

struct Call {
    Call(std::string name) : _name(name) {}
    Call(Call&&) = default;
    Call& operator=(Call&&) = default;
    std::string _name;
};

struct Begin {
    Begin(std::unique_ptr<ASTNode> statement, NodeContainer statements) : _statement(std::move(statement)), _statements(std::move(statements)) {}
    Begin(Begin&&) = default;
    Begin& operator=(Begin&&) = default;
    std::unique_ptr<ASTNode> _statement;
    NodeContainer _statements;
};

struct If {
    If(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> statement) : _condition(std::move(condition)), _statement(std::move(statement)) {}
    If(If&&) = default;
    If& operator=(If&&) = default;
    std::unique_ptr<ASTNode> _condition;
    std::unique_ptr<ASTNode> _statement;
};

struct While {
    While(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> statement) : _condition(std::move(condition)), _statement(std::move(statement)) {}
    While(While&&) = default;
    While& operator=(While&&) = default;
    std::unique_ptr<ASTNode> _condition;
    std::unique_ptr<ASTNode> _statement;
};

struct Condition {
    Condition(TOKEN op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
    Condition(Condition&&) = default;
    Condition& operator=(Condition&&) = default;
    TOKEN _op;
    std::unique_ptr<ASTNode> _left;
    std::unique_ptr<ASTNode> _right;
};

struct OddCondition {
    OddCondition(std::unique_ptr<ASTNode> expression) : _expression(std::move(expression)) {}
    OddCondition(OddCondition&&) = default;
    OddCondition& operator=(OddCondition&&) = default;
    std::unique_ptr<ASTNode> _expression;
};

struct Comparison {
    Comparison(TOKEN op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
    Comparison(Comparison&&) = default;
    Comparison& operator=(Comparison&&) = default;
    TOKEN _op;
    std::unique_ptr<ASTNode> _left;
    std::unique_ptr<ASTNode> _right;
};

struct Expression {
    Expression(TOKEN op, std::unique_ptr<ASTNode> term, ExprContainer terms) : _op(op), _term(std::move(term)), _terms(std::move(terms)) {}
    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;
    TOKEN _op;
    std::unique_ptr<ASTNode> _term;
    ExprContainer _terms;
};

struct Term {
    Term(TOKEN op, std::unique_ptr<ASTNode> left, ExprContainer right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
    Term(Term&&) = default;
    Term& operator=(Term&&) = default;
    TOKEN _op;
    std::unique_ptr<ASTNode> _left;
    ExprContainer _right;
};

struct Factor {
    Factor(std::unique_ptr<ASTNode> value) : _value(std::move(value)) {}
    Factor(Factor&&) = default;
    Factor& operator=(Factor&&) = default;
    std::unique_ptr<ASTNode> _value;
};

struct Program {
    Program(std::unique_ptr<ASTNode> block) : _block(std::move(block)) {}
    Program(Program&&) = default;
    Program& operator=(Program&&) = default;
    std::unique_ptr<ASTNode> _block;
};


class ASTNode {
public:
    template <typename T>
    ASTNode(std::size_t linum, std::size_t column, T &&value);
    template <typename Visitor> auto accept(Visitor &&visitor) const;

    std::size_t _linum;
    std::size_t _column;
    std::variant<
Block,
        ConstDecl,
        VarDecl,
        Procedure,
        Statement,
        Assignment,
        Call,
        Begin,
        If,
        While,
        Condition,
        OddCondition,
        Comparison,
        Expression,
        Term,
        Factor,
        Program
    > _value;
};

template <typename T>
ASTNode::ASTNode(std::size_t linum,
                 std::size_t column, T &&value)
     :_linum(linum), _column(column),
      _value(std::move(value)) {}

template <typename Visitor> auto ASTNode::accept(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), _value);
}

template <typename T, typename... Args>
std::unique_ptr<ASTNode> make_ast_node(std::size_t linum,
                                       std::size_t column, Args &&...args) {
  return std::make_unique<ASTNode>(linum, column,
                                   T{std::forward<Args>(args)...});
}

} // namespace plzerow
