#pragma once

#include <cstdint>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>

namespace plzerow {

struct PrintVisitor {
  PrintVisitor(std::ostream &os) : _os(os) {};
  std::ostream &_os;
  void operator()(double v) { _os << v; }
  void operator()(std::uint32_t v) { _os << v; }
};

using Value = std::variant<double, std::uint32_t>;

using ValueContainer = std::vector<Value>;
using ValuePointer = ValueContainer::const_pointer;

class ValueArray {
public:
  const std::vector<Value> &values() const;
  template <typename T> T at(std::size_t index) const;

  template <typename Visitor>
  auto visit(std::size_t index, Visitor &&visitor) const;
  std::size_t append(const Value &value);

private:
  std::vector<Value> _values;
};

template <typename T> T ValueArray::at(std::size_t index) const {
  return std::get<T>(_values[index]);
}

template <typename Visitor>
auto ValueArray::visit(std::size_t index, Visitor &&visitor) const {
  return std::visit(std::forward<Visitor>(visitor), _values[index]);
}

} // namespace plzerow
