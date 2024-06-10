#pragma once

#include "virtual_machine.hpp"
#include <cstdint>
#include <variant>
#include <vector>

namespace gbpl0 {

/* using Value = double; */
using Value = std::variant<double, std::uint32_t>;

class ValueArray {
public:
  const std::vector<Value> &values() const;
  template <typename T> T at(std::size_t index) const;
  std::size_t append(const Value &value);

private:
  std::vector<Value> _values;
};

template <typename T> T ValueArray::at(std::size_t index) const {
  return std::get<T>(_values[index]);
}

} // namespace gbpl0
