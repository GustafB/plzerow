#pragma once

namespace plzerow {

enum class TOKEN {
  PROGRAM,
  IDENT = 'I',
  NUMBER = 'N',
  CONST = 'C',
  VAR = 'V',
  PROCEDURE = 'P',
  CALL = 'c',
  BEGIN = 'B',
  END = 'E',
  IF = 'i',
  THEN = 'T',
  WHILE = 'W',
  DO = 'D',
  ODD = 'O',
  DOT = '.',
  EQUAL = '=',
  COMMA = ',',
  SEMICOLON = ';',
  ASSIGN = ':',
  HASH = '#',
  LT = '<',
  GT = '>',
  PLUS = '+',
  MINUS = '-',
  MULTIPLY = '*',
  DIVIDE = '/',
  LPAREN = '(',
  RPAREN = ')',
  NOT = '!',
  BANG_EQUAL,
  GE,
  LE,
  EQUAL_EQUAL,
  ENDFILE = '\0',
  ERROR,
  UNKNOWN,
};

}
