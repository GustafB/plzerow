import sys

ast_nodes = [
    "Block     : NodeContainer constDecls | NodeContainer varDecls | NodeContainer procedures | std::unique_ptr<ASTNode> statement",
    "ConstDecl : std::string name | int value",
    "VarDecl   : std::string name",
    "Procedure : std::string name | std::unique_ptr<ASTNode> block",
    "Statement : std::unique_ptr<ASTNode> statement",
    "Assignment: std::string name | std::unique_ptr<ASTNode> expression",
    "Call      : std::string name",
    "Begin     : std::unique_ptr<ASTNode> statement | NodeContainer statements",
    "If        : std::unique_ptr<ASTNode> condition | std::unique_ptr<ASTNode> statement",
    "While     : std::unique_ptr<ASTNode> condition | std::unique_ptr<ASTNode> statement",
    "Condition : TOKEN op | std::unique_ptr<ASTNode> left | std::unique_ptr<ASTNode> right",
    "OddCondition: std::unique_ptr<ASTNode> expression",
    "Comparison: TOKEN op | std::unique_ptr<ASTNode> left | std::unique_ptr<ASTNode> right",
    "Expression: TOKEN op | std::unique_ptr<ASTNode> left | ExprContainer right",
    "Term      : TOKEN op | std::unique_ptr<ASTNode> left | ExprContainer right",
    "Binary    : TOKEN op | std::unique_ptr<ASTNode> left | std::unique_ptr<ASTNode> right",
    "Unary     : TOKEN op | std::unique_ptr<ASTNode> right",
    "Factor    : TOKEN op | std::unique_ptr<ASTNode> right",
    "Primary   : std::string right",
    "Program   : std::unique_ptr<ASTNode> block",
]


def generate_header(nodes):
    header = """
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

"""

    def needs_move(type):
        for t in ("std::unique_ptr", "NodeContainer", "ExprContainer"):
            if t in type:
                return True
        return False

    node_classes = []
    for node in nodes:
        parts = node.split(":", 1)
        class_name = parts[0].strip()
        node_classes.append(class_name)

    for class_name in node_classes:
        header += f"struct {class_name};\n"

    header += "\n"

    for node in nodes:
        parts = node.split(":", 1)
        struct_name = parts[0].strip()
        fields = [f.strip() for f in parts[1].split("|")]

        header += f"struct {struct_name} {{\n"

        header += "    " + struct_name + "("
        header += ", ".join(
            [f"{f.rsplit(' ', 1)[0]} {f.rsplit(' ', 1)[1]}" for f in fields]
        )
        header += ") : "
        header += ", ".join(
            [
                f"_{f.rsplit(' ', 1)[1]}(std::move({f.rsplit(' ', 1)[1]}))"
                if needs_move(f.rsplit(" ", 1)[0])
                else f"_{f.rsplit(' ', 1)[1]}({f.rsplit(' ', 1)[1]})"
                for f in fields
            ]
        )
        header += " {}\n"

        header += f"    {struct_name}({struct_name}&&) = default;\n"
        header += f"    {struct_name}& operator=({struct_name}&&) = default;\n"

        for field in fields:
            try:
                ftype, fname = field.rsplit(" ", 1)
                header += f"    {ftype} _{fname};\n"
            except ValueError:
                print(f"Error parsing field '{field}' in class '{struct_name}'")

        header += "};\n\n"

    header += """
class ASTNode {
public:
    template <typename T>
    ASTNode(std::size_t linum, std::size_t column, T &&value);
    template <typename Visitor> auto accept(Visitor &&visitor) const;

    std::size_t _linum;
    std::size_t _column;
    std::variant<
"""
    header += ",\n        ".join(f"{class_name}" for class_name in node_classes)
    header += """
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
"""

    return header


def generate_source(nodes):
    source = """
#include "ast_nodes.hpp"

namespace plzerow {

} // namespace plzerow
"""
    return source


def write_files(include_dir, src_dir):
    header_content = generate_header(ast_nodes)
    with open(f"{include_dir}/ast_nodes.hpp", "w") as header_file:
        header_file.write(header_content)

    source_content = generate_source(ast_nodes)
    with open(f"{src_dir}/ast_nodes.cpp", "w") as source_file:
        source_file.write(source_content)

    print("Header and source files generated successfully.")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: script.py <include_dir> <src_dir>")
        sys.exit(1)

    include_directory = sys.argv[1]
    source_directory = sys.argv[2]

    write_files(include_directory, source_directory)
