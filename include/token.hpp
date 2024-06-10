#pragma once

namespace gbpl0 {

enum class TOKEN {
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
  LESSTHAN = '<',
  GREATERTHAN = '>',
  PLUS = '+',
  MINUS = '-',
  MULTIPLY = '*',
  DIVIDE = '/',
  LPAREN = '(',
  RPAREN = ')',
  ENDFILE = '\0',
  UNKNOWN,
};

}