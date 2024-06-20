#pragma once

#include <memory>
#include <plzerow/chunk.hpp>
#include <string>
#include "ast_nodes.hpp"

namespace plzerow {
#ifdef PLZEROW_DEBUG
class Debugger {
 public:
  static std::size_t disassemble_instruction(std::size_t offset,
                                             const Chunk &chunk);
  static std::size_t disassemble(const std::string &name, const Chunk &chunk);

  static std::string ast_to_npn(const std::unique_ptr<ASTNode> &root);
};
#endif
}  // namespace plzerow
