#include "virtual_machine.hpp"
#include "chunk.hpp"
#include "debugger.hpp"
#include <iostream>

namespace plzerow {

InstructionPointer VM::next() { return _ip++; }

InterpretResult VM::run() {
  for (;;) {
    disassemble_instruction(_ip - _chunk.cbegin(), _chunk);
    std::uint8_t instruction;
    switch (instruction = *next()) {
    case OP_CONSTANT:
    case OP_CONSTANT_LONG:
      next();
      break;
    case OP_RETURN:
      return InterpretResult::OK;
    default:
      std::cout << "COMPILE_ERROR\n";
      return InterpretResult::COMPILE_ERROR;
    }
  }

  return InterpretResult::OK;
}

} // namespace plzerow
