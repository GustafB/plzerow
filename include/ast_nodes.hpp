
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "token_type.hpp"

namespace plzerow {

class ASTNode;

class Block;
class ConstDecl;
class VarDecl;
class Procedure;
class Statement;
class Assignment;
class Call;
class Begin;
class If;
class While;
class Condition;
class OddCondition;
class Comparison;
class Expression;
class Term;
class Factor;
class Program;

class Block {
    std::vector<std::unique_ptr<ASTNode>> _constDecls;
    std::vector<std::unique_ptr<ASTNode>> _varDecls;
    std::vector<std::unique_ptr<ASTNode>> _procedures;
    std::unique_ptr<ASTNode> _statement;
public:
    Block(std::vector<std::unique_ptr<ASTNode>> constDecls, std::vector<std::unique_ptr<ASTNode>> varDecls, std::vector<std::unique_ptr<ASTNode>> procedures, std::unique_ptr<ASTNode> statement) : _constDecls(std::move(constDecls)), _varDecls(std::move(varDecls)), _procedures(std::move(procedures)), _statement(std::move(statement)) {}
};

class ConstDecl {
    std::string _name;
    int _value;
public:
    ConstDecl(std::string name, int value) : _name(name), _value(value) {}
};

class VarDecl {
    std::string _name;
public:
    VarDecl(std::string name) : _name(name) {}
};

class Procedure {
    std::string _name;
    std::unique_ptr<ASTNode> _block;
public:
    Procedure(std::string name, std::unique_ptr<ASTNode> block) : _name(name), _block(std::move(block)) {}
};

class Statement {
    std::vector<std::unique_ptr<ASTNode>> _statement;
public:
    Statement(std::vector<std::unique_ptr<ASTNode>> statement) : _statement(std::move(statement)) {}
};

class Assignment {
    std::string _name;
    std::unique_ptr<ASTNode> _expression;
public:
    Assignment(std::string name, std::unique_ptr<ASTNode> expression) : _name(name), _expression(std::move(expression)) {}
};

class Call {
    std::string _name;
public:
    Call(std::string name) : _name(name) {}
};

class Begin {
    std::unique_ptr<ASTNode> _statement;
    std::vector<std::unique_ptr<ASTNode>> _statements;
public:
    Begin(std::unique_ptr<ASTNode> statement, std::vector<std::unique_ptr<ASTNode>> statements) : _statement(std::move(statement)), _statements(std::move(statements)) {}
};

class If {
    std::unique_ptr<ASTNode> _condition;
    std::unique_ptr<ASTNode> _statement;
public:
    If(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> statement) : _condition(std::move(condition)), _statement(std::move(statement)) {}
};

class While {
    std::unique_ptr<ASTNode> _condition;
    std::unique_ptr<ASTNode> _statement;
public:
    While(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> statement) : _condition(std::move(condition)), _statement(std::move(statement)) {}
};

class Condition {
    std::unique_ptr<ASTNode> _left;
    std::unique_ptr<ASTNode> _right;
    char _op;
public:
    Condition(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, char op) : _left(std::move(left)), _right(std::move(right)), _op(op) {}
};

class OddCondition {
    std::unique_ptr<ASTNode> _expression;
public:
    OddCondition(std::unique_ptr<ASTNode> expression) : _expression(std::move(expression)) {}
};

class Comparison {
    std::optional<char> _op;
    std::unique_ptr<ASTNode> _left;
    std::unique_ptr<ASTNode> _right;
public:
    Comparison(std::optional<char> op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
};

class Expression {
    std::unique_ptr<ASTNode> _term;
    std::vector<std::pair<std::string, std::unique_ptr<ASTNode>>> _terms;
public:
    Expression(std::unique_ptr<ASTNode> term, std::vector<std::pair<std::string, std::unique_ptr<ASTNode>>> terms) : _term(std::move(term)), _terms(std::move(terms)) {}
};

class Term {
    std::unique_ptr<ASTNode> _left;
    std::vector<std::pair<char, std::unique_ptr<ASTNode>>> _right;
public:
    Term(std::unique_ptr<ASTNode> left, std::vector<std::pair<char, std::unique_ptr<ASTNode>>> right) : _left(std::move(left)), _right(std::move(right)) {}
};

class Factor {
    std::unique_ptr<ASTNode> _value;
public:
    Factor(std::unique_ptr<ASTNode> value) : _value(std::move(value)) {}
};

class Program {
    std::unique_ptr<ASTNode> _block;
public:
    Program(std::unique_ptr<ASTNode> block) : _block(std::move(block)) {}
};


class ASTNode {
public:
    ASTNode(TOKEN token_type, std::size_t linum, std::size_t column);
    template <typename Visitor> auto accept(Visitor &&visitor) const;

    TOKEN _type;
    std::size_t _column;
    std::size_t _linum;
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

template <typename Visitor> auto ASTNode::accept(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), _value);
}

} // namespace plzerow