ast_nodes = [
    "Block     : std::vector<std::unique_ptr<ASTNode>> constDecls | std::vector<std::unique_ptr<ASTNode>> varDecls | std::vector<std::unique_ptr<ASTNode>> procedures | std::unique_ptr<ASTNode> statement",
    "ConstDecl : std::string name | int value",
    "VarDecl   : std::string name",
    "Procedure : std::string name | std::unique_ptr<ASTNode> block",
    "Statement : std::unique_ptr<ASTNode> statement",
    "Assignment: std::string name | std::unique_ptr<ASTNode> expression",
    "Call      : std::string name",
    "BeginEnd  : std::vector<std::unique_ptr<ASTNode>> statements",
    "If        : std::unique_ptr<ASTNode> condition | std::unique_ptr<ASTNode> statement",
    "While     : std::unique_ptr<ASTNode> condition | std::unique_ptr<ASTNode> statement",
    "Condition : std::unique_ptr<ASTNode> condition",
    "OddCondition: std::unique_ptr<ASTNode> expression",
    "Comparison: std::string op | std::unique_ptr<ASTNode> left | std::unique_ptr<ASTNode> right",
    "Expression: std::unique_ptr<ASTNode> term | std::vector<std::pair<std::string, std::unique_ptr<ASTNode>>> terms",
    "Term      : std::unique_ptr<ASTNode> factor | std::vector<std::pair<std::string, std::unique_ptr<ASTNode>>> factors",
    "Factor    : std::variant<std::string, int, std::unique_ptr<ASTNode>> factor",
    "Program   : std::unique_ptr<ASTNode> block",
]


# Function to generate header file content
def generate_header(nodes):
    header = """
#ifndef AST_NODES_H
#define AST_NODES_H

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "token_type.hpp"

namespace plzerow {

class ASTNode;

"""

    node_classes = []
    for node in nodes:
        parts = node.split(":", 1)
        class_name = parts[0].strip()
        node_classes.append(class_name)

    # Generate forward declarations for all node classes
    for class_name in node_classes:
        header += f"class {class_name};\n"

    header += "\n"

    # Generate structs for each node type
    for node in nodes:
        parts = node.split(":", 1)
        struct_name = parts[0].strip()
        fields = [f.strip() for f in parts[1].split("|")]

        header += f"class {struct_name} {{\n"

        # Fields
        for field in fields:
            try:
                ftype, fname = field.rsplit(" ", 1)
                header += f"    {ftype} _{fname};\n"
            except ValueError:
                print(f"Error parsing field '{field}' in class '{struct_name}'")

        # Constructor
        header += "public:\n"
        header += "    " + struct_name + "("
        header += ", ".join(
            [f"{f.rsplit(' ', 1)[0]} {f.rsplit(' ', 1)[1]}" for f in fields]
        )
        header += ") : "
        header += ", ".join(
            [
                f"_{f.rsplit(' ', 1)[1]}(std::move({f.rsplit(' ', 1)[1]}))"
                if "std::unique_ptr" in f.rsplit(" ", 1)[0]
                else f"_{f.rsplit(' ', 1)[1]}({f.rsplit(' ', 1)[1]})"
                for f in fields
            ]
        )
        header += " {}\n"

        header += "};\n\n"

    # Generate ASTNode class definition
    header += """
class ASTNode {
public:
    ASTNode(TOKEN token_type, std::size_t linum, std::size_t column);
    template <typename Visitor> auto accept(Visitor &&visitor) const;

    TOKEN _type;
    std::size_t _column;
    std::size_t _linum;
    std::variant<
"""
    header += ",\n        ".join(f"{class_name}" for class_name in node_classes)
    header += """
    > _value;
};

template <typename Visitor> auto ASTNode::accept(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), _value);
}

} // namespace plzerow
"""

    header += "#endif // AST_NODES_H\n"
    return header


# Function to generate source file content
def generate_source(nodes):
    source = """
#include "ast_nodes.hpp"

namespace plzerow {

ASTNode::ASTNode(TOKEN token_type, std::size_t linum, std::size_t column)
    : _type(token_type), _linum(linum), _column(column) {}

} // namespace plzerow
"""
    return source


# Write header file
header_content = generate_header(ast_nodes)
with open("ast_nodes.hpp", "w") as header_file:
    header_file.write(header_content)

# Write source file
source_content = generate_source(ast_nodes)
with open("ast_nodes.cpp", "w") as source_file:
    source_file.write(source_content)

print("Header and source files generated successfully.")
