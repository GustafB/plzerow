#pragma once

#include <istream>
#include <string>
#include <vector>

namespace plzerow {

class InputHandler {
public:
  static std::vector<char> read_from_repl(std::istream &is);
  static std::vector<char> read_from_file(const std::string &filename);
};
} // namespace plzerow
