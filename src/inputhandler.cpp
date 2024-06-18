#include "inputhandler.hpp"
#include <fstream>
#include <iostream>
#include <istream>

namespace plzerow {

std::vector<char> InputHandler::read_from_file(const std::string &filename) {
  auto filestream = std::ifstream(filename, std::ios::binary | std::ios::ate);

  if (!filestream) {
    std::cerr << "unable to open source file: " << filename << "\n";
    exit(1);
  }
  filestream.seekg(0, std::ios::end);
  const auto filesize = filestream.tellg();

  filestream.seekg(0, std::ios::beg);

  std::vector<char> buffer{};

  buffer.resize(filesize);

  filestream.read(buffer.data(), filesize);

  if (!filestream) {
    std::cerr << "unable to read source file into buffer\n";
    exit(1);
  }

  filestream.close();

  std::cout << "Filename: " << filename << "\nFile Size: " << filesize << "\n";

  return buffer;
}

std::vector<char> InputHandler::read_from_repl(std::istream &is) {
  std::string current_line;
  std::getline(is, current_line);
  return {current_line.begin(), current_line.end()};
}

} // namespace plzerow
