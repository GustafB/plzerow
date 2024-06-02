#include "lexer.hpp"
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

void help() { std::cout << "usage: pl0 file.pl0\n"; }

int main(int argc, char **argv) {
  using namespace gbpl0;
  if (argc != 2) {
    help();
    exit(1);
  }

  auto lexer = Lexer();
  const auto filename = std::string(argv[1]);
  lexer.ingest_buffer(filename);
  TOKEN token;
  while ((token = lexer.next_token()) != TOKEN::ENDFILE) {
    std::cout << "[" << filename << "]" << lexer.linum() << ":"
              << lexer.tstart() << ": " << static_cast<char>(token) << " = '"
              << lexer.token() << "'"
              << "\n";
  }
  std::cout << "EOF\n";

  return 0;
}
