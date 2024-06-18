
#ifndef AST_NODES_H
#define AST_NODES_H

#include <memory>
#include <string>
#include <variant>

// Forward declarations
class Expr;
class Token;

class Binary {
    std::unique_ptr<Expr> left;
    std::unique_ptr<Token> operator;
    std::unique_ptr<Expr> right;
public:
    Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> operator, std::unique_ptr<Expr> right) : left(std::move(left)), operator(std::move(operator)), right(std::move(right)) {}
};

class Grouping {
    std::unique_ptr<Expr> expression;
public:
    Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

class Literal {
    std::unique_ptr<Object> value;
public:
    Literal(std::unique_ptr<Object> value) : value(std::move(value)) {}
};

class Unary {
    std::unique_ptr<Token> operator;
    std::unique_ptr<Expr> right;
public:
    Unary(std::unique_ptr<Token> operator, std::unique_ptr<Expr> right) : operator(std::move(operator)), right(std::move(right)) {}
};

enum class NodeKind { Binary, Grouping, Literal, Unary };
enum class TOKEN;


class ASTNode {
public:
    ASTNode(TOKEN token_type, std::size_t linum, std::size_t column, NodeKind kind);
    template <typename Visitor> auto accept(Visitor &&visitor) const;

    NodeKind _kind;
    TOKEN _type;
    std::size_t _column;
    std::size_t _linum;
    std::variant<
Binary,
        Grouping,
        Literal,
        Unary
    > _value;
};

template <typename Visitor> auto ASTNode::accept(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), _value);
}
#endif // AST_NODES_H
