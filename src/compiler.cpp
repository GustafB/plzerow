#include "compiler.hpp"
#include "chunk.hpp"
#include "parser.hpp"
#include "value.hpp"
#include <cstdint>
#include <memory>
#include <utility>

namespace plzerow {

CompilerResult Compiler::compile(std::vector<char> &&source_code) {
  _lexer = Lexer(std::forward<std::vector<char>>(source_code));
  _parser = Parser([this]() { return _lexer.next(); });
  _ast = _parser.parse();
  print(_ast);
  return CompilerResult::OK;
}

void Compiler::print(const std::unique_ptr<ASTNode> &node) const {
  static const Visitor printVisitor{
      [this](const Block &arg) -> void {
        // std::cout << "size = " << arg._varDecls.size() << "\n";
        for (const auto &c : arg._constDecls) {
          print(c);
        }
        for (const auto &v : arg._varDecls) {
          print(v);
        }
        for (const auto &p : arg._procedures) {
          print(p);
        }
        print(arg._statement);
      },
      [this](const ConstDecl &arg) -> void {
        std::cout << arg._name << " " << arg._value << "\n";
      },
      [this](const VarDecl &arg) -> void { std::cout << arg._name << "\n"; },
      [this](const Procedure &arg) -> void { std::cout << "Procedure\n"; },
      [this](const Statement &arg) -> void { print(arg._statement); },
      [this](const Assignment &arg) -> void { std::cout << "Assignment\n"; },
      [this](const Call &arg) -> void { std::cout << "Call\n"; },
      [this](const Begin &arg) -> void {
        print(arg._statement);
        for (const auto &s : arg._statements) {
          print(s);
        }
      },
      [this](const If &arg) -> void { std::cout << "If\n"; },
      [this](const While &arg) -> void { std::cout << "While\n"; },
      [this](const Condition &arg) -> void { std::cout << "Condition\n"; },
      [this](const OddCondition &arg) -> void {
        std::cout << "OddCondition\n";
      },
      [this](const Comparison &arg) -> void { std::cout << "Comparison\n"; },
      [this](const Expression &arg) -> void { std::cout << "Expression\n"; },
      [this](const Term &arg) -> void { std::cout << "Term\n"; },
      [this](const Binary &arg) -> void { std::cout << "Binary\n"; },
      [this](const Unary &arg) -> void { std::cout << "Unary\n"; },
      [this](const Factor &arg) -> void { std::cout << "Factor\n"; },
      [this](const Primary &arg) -> void { std::cout << "Primary\n"; },
      [this](const Program &arg) -> void { print(arg._block); },
  };
  node->accept(printVisitor);
}

// void Compiler::emit_byte(std::uint8_t byte);

} // namespace plzerow
