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

void help() { std::cout << "usage: pl0 [file.pl0]\n"; }

int main(int argc, char *argv[]) {
  VM vm;
  if (argc == 1) {
    vm.repl();
  } else if (argc == 2) {
    const std::string filename{argv[1]};
    vm.runfile(filename);
  } else {
    help();
    exit(1);
  }
}
