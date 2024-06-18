#include "ast.hpp"
#include <variant>

namespace plzerow {

ASTNode::ASTNode(TOKEN token_type, std::size_t linum, std::size_t column)
    : _type{token_type}, _column{column}, _linum{linum} {}

// Unary::Unary(TOKEN token_type, std::unique_ptr<ASTNode> rhs, std::size_t
// linum,
//              std::size_t column)
//     : ASTNode(token_type, linum, column), _rhs{std::move(rhs)} {}

// Binary::Binary(TOKEN token_type, std::unique_ptr<ASTNode> lhs,
//                std::unique_ptr<ASTNode> rhs, std::size_t linum,
//                std::size_t column)
//     : ASTNode(token_type, linum, column), _lhs{std::move(lhs)},
//       _rhs{std::move(rhs)} {}

} // namespace plzerow
