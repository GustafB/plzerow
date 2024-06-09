#include "filehandler.hpp"
#include <fstream>
#include <iostream>

namespace gbpl0 {

bool FileHandler::validate(const std::string &filename) const {
  const std::string file_extension = ".pl0";
  if (filename.length() >= file_extension.size()) {
    if (filename.rfind(file_extension) !=
        (filename.length() - file_extension.length())) {
      return false;
    }
  }
  return true;
}

std::vector<char> FileHandler::read_file(const std::string &filename) const {
  if (!validate(filename)) {
    std::cerr << "file is not of the right type\n";
    exit(1);
  }
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

} // namespace gbpl0
