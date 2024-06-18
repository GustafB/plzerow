
#include "ast_nodes.hpp"

namespace plzerow {

ASTNode::ASTNode(TOKEN token_type, std::size_t linum, std::size_t column)
    : _type(token_type), _linum(linum), _column(column) {}

} // namespace plzerow
