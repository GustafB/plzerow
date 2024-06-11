#include "chunk.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "virtual_machine.hpp"
#include <cstdint>
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

using namespace gbpl0;

void help() { std::cout << "usage: pl0 file.pl0\n"; }

int main() {
  Chunk c;
  std::uint32_t v = 123456789;
  c.append(OP_CODE::OP_CONSTANT, Value{123.5}, 123);
  c.append(OP_CODE::OP_CONSTANT, Value{456.12}, 123);
  c.append(OP_CODE::OP_RETURN, 123);
  c.append(OP_CODE::OP_CONSTANT, Value{789.32}, 123);
  c.append(OP_CODE::OP_CONSTANT_LONG, Value{v}, 456);
  c.append(OP_CODE::OP_RETURN, 456);
  c.disassemble("test chunk");
}
