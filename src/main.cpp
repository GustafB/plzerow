#include "chunk.hpp"
#include "virtual_machine.hpp"
#include <cstdlib>
#include <iostream>

/*
 * pl0c -- PL/0 compiler.
 *
 * program	= block "." .
 * block	= [ "const" ident "=" number { "," ident "=" number } ";" ]
 *		  [ "var" ident { "," ident } ";" ]
 *		  { "procedure" ident ";" block ";" } statement .
 * statement	= [ ident ":=" expression
 *		  | "call" ident
 *		  | "begin" statement { ";" statement } "end"
 *		  | "if" condition "then" statement
 *		  | "while" condition "do" statement ] .
 * condition	= "odd" expression
 *		| expression ( "=" | "#" | "<" | ">" ) expression .
 * expression	= [ "+" | "-" ] term { ( "+" | "-" ) term } .
 * term		= factor { ( "*" | "/" ) factor } .
 * factor	= ident
 *		| number
 *		| "(" expression ")" .*
 *
 *
 */

using namespace plzerow;

void help() { std::cout << "usage: pl0 file.pl0\n"; }

int main() {
  Chunk c;
  c.append(OP_CODE::OP_CONSTANT, Value{100}, 123);
  c.append(OP_CODE::OP_CONSTANT, Value{100}, 123);
  c.append(OP_CODE::OP_ADD, 1000);
  c.append(OP_CODE::OP_CONSTANT, Value{2}, 1000);
  c.append(OP_CODE::OP_MULTIPLY, 1000);
  c.append(OP_CODE::OP_CONSTANT, Value{100}, 1000);
  c.append(OP_CODE::OP_SUBTRACT, 1000);
  c.append(OP_CODE::OP_CONSTANT, Value{300}, 1000);
  c.append(OP_CODE::OP_ADD, 1000);
  c.append(OP_CODE::OP_CONSTANT, Value{2}, 1000);
  c.append(OP_CODE::OP_DIVIDE, 1000);
  c.append(OP_CODE::OP_RETURN, 1000);

  VM vm(std::move(c));
  vm.run();
}
