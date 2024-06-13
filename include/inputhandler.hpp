#pragma once

#include <istream>
#include <string>
#include <vector>

namespace plzerow {

class InputHandler {
public:
  std::vector<char> read_from_repl(std::istream &is) const;
  std::vector<char> read_from_file(const std::string &filename) const;
};
} // namespace plzerow
