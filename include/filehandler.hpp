#pragma once

#include <string>
#include <vector>

namespace plzerow {

class FileHandler {
public:
  FileHandler() = default;
  FileHandler(const std::string &extension) : _extension(extension) {}
  std::vector<char> read_file(const std::string &filename) const;

private:
  bool validate(const std::string &filename) const;
  std::string _extension;
};
} // namespace plzerow
